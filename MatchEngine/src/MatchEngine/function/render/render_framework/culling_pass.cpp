#include <MatchEngine/function/render/render_framework/culling_pass.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    void CullingPass::createRenderResource(Match::RenderPassBuilder &builder) {
        builder.add_attachment("depth", Match::AttachmentType::eDepthBuffer);
    }

    void CullingPass::postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto manager = global_runtime_context->render_system->getMatchAPIManager();
        auto factory = global_runtime_context->render_system->getMatchFactory();
        auto max_primitive_count = global_runtime_context->assets_system->getMaxPrimitiveCount();
        auto max_mesh_instance_count = global_runtime_context->render_system->getMaxMeshInstanceCount();
        auto [width, height] = Match::runtime_setting->get_window_size();
        resource.depth_mip_level_count = glm::floor(glm::log2<float>(glm::max(width, height))) + 1;

        depth_descriptor_set = factory->create_descriptor_set();
        depth_descriptor_set->add_descriptors({
            { Match::ShaderStage::eCompute, 0, Match::DescriptorType::eTexture, 1 },
            { Match::ShaderStage::eCompute, 1, Match::DescriptorType::eStorageImage, resource.depth_mip_level_count },
            { Match::ShaderStage::eCompute, 2, Match::DescriptorType::eStorageImage, resource.depth_mip_level_count },
        }).allocate();

        depth_constants = factory->create_push_constants(
            Match::ShaderStage::eCompute,
            {
                { "image_size", Match::ConstantType::eUint32x2 },
                { "mip_level", Match::ConstantType::eUint32 }
            });

        vk::ImageMemoryBarrier image_barrier {};
        image_barrier.setImage(renderer->framebuffer_set->attachments.at(renderer->render_pass_builder->get_attachment_index("depth", true)).image->image)
            .setSrcAccessMask(vk::AccessFlagBits::eNone)
            .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
            .setOldLayout(vk::ImageLayout::eUndefined)
            .setNewLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
            .setSubresourceRange({
                vk::ImageAspectFlagBits::eDepth | (Match::has_stencil_component(Match::get_supported_depth_format()) ? vk::ImageAspectFlagBits::eStencil : vk::ImageAspectFlagBits::eNone) ,
                0,
                1,
                0,
                1
            }); 
        auto command_buffer = manager->command_pool->allocate_single_use();
        command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eEarlyFragmentTests, {}, {}, {}, image_barrier);
        manager->command_pool->free_single_use(command_buffer);
        
        vk::SamplerReductionModeCreateInfo sampler_reduction_mode {};
        sampler_reduction_mode.setReductionMode(vk::SamplerReductionMode::eMax);
        vk::SamplerCreateInfo sampler_create_info {};
        sampler_create_info.setMagFilter(vk::Filter::eNearest)
            .setMinFilter(vk::Filter::eNearest)
            .setMipmapMode(vk::SamplerMipmapMode::eNearest)
            .setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
            .setAddressModeV(vk::SamplerAddressMode::eClampToEdge)
            .setAddressModeW(vk::SamplerAddressMode::eClampToEdge)
            .setMinLod(0)
            .setMaxLod(resource.depth_mip_level_count - 1)
            .setPNext(&sampler_reduction_mode);
        resource.depth_sampler = manager->device->device.createSampler(sampler_create_info);
        renderer->register_resource_recreate_callback([&, ptr = renderer.get()]() {
            vk::ImageMemoryBarrier image_barrier {};
            image_barrier.setImage(ptr->framebuffer_set->attachments.at(ptr->render_pass_builder->get_attachment_index("depth", true)).image->image)
                .setSrcAccessMask(vk::AccessFlagBits::eNone)
                .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
                .setOldLayout(vk::ImageLayout::eUndefined)
                .setNewLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
                .setSubresourceRange({
                    vk::ImageAspectFlagBits::eDepth | (Match::has_stencil_component(Match::get_supported_depth_format()) ? vk::ImageAspectFlagBits::eStencil : vk::ImageAspectFlagBits::eNone) ,
                    0,
                    1,
                    0,
                    1
                }); 
            auto command_buffer = global_runtime_context->render_system->getMatchAPIManager()->command_pool->allocate_single_use();
            command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eEarlyFragmentTests, {}, {}, {}, image_barrier);
            global_runtime_context->render_system->getMatchAPIManager()->command_pool->free_single_use(command_buffer);

            for (size_t in_flight_index = 0; in_flight_index < Match::setting.max_in_flight_frame; in_flight_index ++) {
                vk::DescriptorImageInfo image_info {};
                image_info.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                    .setImageView(ptr->framebuffer_set->attachments.at(ptr->render_pass_builder->get_attachment_index("depth", true)).image_view)
                    .setSampler(resource.depth_sampler);
                vk::WriteDescriptorSet descriptor_write {};
                descriptor_write.setDstSet(depth_descriptor_set->descriptor_sets[in_flight_index])
                    .setDstBinding(0)
                    .setDstArrayElement(0)
                    .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                    .setImageInfo(image_info);
                global_runtime_context->render_system->getMatchAPIManager()->device->device.updateDescriptorSets({ descriptor_write }, {});
            }
        });
        
        for (size_t in_flight_index = 0; in_flight_index < Match::setting.max_in_flight_frame; in_flight_index ++) {
            auto image = std::make_shared<Match::Image>(width, height, vk::Format::eR32Sfloat, vk::ImageUsageFlagBits::eStorage | vk::ImageUsageFlagBits::eSampled, vk::SampleCountFlagBits::e1, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, resource.depth_mip_level_count);
            Match::transition_image_layout(image->image, vk::ImageAspectFlagBits::eColor, resource.depth_mip_level_count, { vk::ImageLayout::eUndefined, vk::AccessFlagBits::eNone, vk::PipelineStageFlagBits::eTopOfPipe }, { vk::ImageLayout::eGeneral, vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite, vk::PipelineStageFlagBits::eComputeShader });
            resource.depth_textures.push_back(image);
            resource.depth_texture_views.push_back(Match::create_image_view(image->image, vk::Format::eR32Sfloat, vk::ImageAspectFlagBits::eColor, resource.depth_mip_level_count));
            resource.depth_texture_mipmap_views.push_back({});
            vk::ImageViewCreateInfo create_info {};
            create_info.setImage(image->image)
                .setFormat(vk::Format::eR32Sfloat)
                .setViewType(vk::ImageViewType::e2D)
                .setComponents({
                    vk::ComponentSwizzle::eR,
                    vk::ComponentSwizzle::eIdentity,
                    vk::ComponentSwizzle::eIdentity,
                    vk::ComponentSwizzle::eIdentity,
                })
                .setSubresourceRange({
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    0,
                    1
                });
            for (uint32_t i = 0; i < resource.depth_mip_level_count; i ++) {
                create_info.subresourceRange.setBaseMipLevel(i);
                resource.depth_texture_mipmap_views.back().push_back(manager->device->device.createImageView(create_info));
            }

            vk::DescriptorImageInfo image_info {};
            image_info.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(renderer->framebuffer_set->attachments.at(renderer->render_pass_builder->get_attachment_index("depth", true)).image_view)
                .setSampler(resource.depth_sampler);
            vk::WriteDescriptorSet descriptor_write {};
            descriptor_write.setDstSet(depth_descriptor_set->descriptor_sets[in_flight_index])
                .setDstBinding(0)
                .setDstArrayElement(0)
                .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                .setImageInfo(image_info);
            manager->device->device.updateDescriptorSets({ descriptor_write }, {});
            std::vector<vk::DescriptorImageInfo> image_infos(resource.depth_mip_level_count);
            for (uint32_t i = 0; i < resource.depth_mip_level_count; i ++) {
                image_infos[i].setImageLayout(vk::ImageLayout::eGeneral)
                    .setImageView(resource.depth_texture_mipmap_views.back()[i]);
            }
            descriptor_write
                // .setDstSet(depth_descriptor_set->descriptor_sets[in_flight_index])
                .setDstBinding(1)
                // .setDstArrayElement(0)
                .setDescriptorType(vk::DescriptorType::eStorageImage)
                .setImageInfo(image_infos);
            manager->device->device.updateDescriptorSets({ descriptor_write }, {});
            descriptor_write.setDstBinding(2);
            manager->device->device.updateDescriptorSets({ descriptor_write }, {});
        }

        copy_depth_texture_shader_program = factory->create_compute_shader_program();
        copy_depth_texture_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/copy_depth_texture.comp", Match::ShaderStage::eCompute))
            .attach_descriptor_set(depth_descriptor_set)
            .attach_push_constants(depth_constants)
            .compile();

        generate_depth_texture_shader_program = factory->create_compute_shader_program();
        generate_depth_texture_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/generate_depth_texture.comp", Match::ShaderStage::eCompute))
            .attach_descriptor_set(depth_descriptor_set)
            .attach_push_constants(depth_constants)
            .compile();
        generate_depth_texture_shader_program->descriptor_sets.clear();
        generate_depth_texture_shader_program->push_constants.reset();

        resource.counts_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * 5, vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_AUTO, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        resource.visible_mesh_instance_indices_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * 2 * max_mesh_instance_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.primitive_counts_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * max_primitive_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.indirect_commands_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(vk::DrawIndexedIndirectCommand) * max_primitive_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.available_indirect_commands_buffer = std::make_shared<Match::InFlightBuffer>(resource.indirect_commands_buffer->get_size(), vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.instance_datas_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(glm::vec4) * 3 * max_mesh_instance_count, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

        resource.culling_command_buffers = manager->command_pool->allocate_command_buffer(Match::setting.max_in_flight_frame);

        vk::SemaphoreCreateInfo create_info {};
        for (size_t in_flight_index = 0; in_flight_index < Match::setting.max_in_flight_frame; in_flight_index ++) {
            resource.culling_finish_semaphores.push_back(manager->device->device.createSemaphore(create_info));
            auto ptr = static_cast<uint32_t *>(resource.counts_buffer->in_flight_buffers[in_flight_index]->map());
            ptr[0] = 0;
            ptr[1] = 1;
            ptr[2] = 1;
            ptr[3] = 0;
            ptr[4] = 0;
        }

        readonly_descriptor_set = factory->create_descriptor_set();
        readonly_descriptor_set->add_descriptors({
            { Match::ShaderStage::eCompute, 0, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 1, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 2, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 3, Match::DescriptorType::eUniform },
        }).allocate()
            .bind_storage_buffer(0, global_runtime_context->render_system->getSwapData()->getMeshInstancePool()->mesh_instance_buffer)
            .bind_storage_buffer(1, global_runtime_context->assets_system->getMeshPool()->mesh_descriptor_buffer)
            .bind_storage_buffer(2, global_runtime_context->assets_system->getMeshPool()->primitive_descriptor_buffer)
            .bind_uniform(3, global_runtime_context->render_system->getSwapData()->getCameraUniformBuffer());
        
        descriptor_set = factory->create_descriptor_set();
        descriptor_set->add_descriptors({
            { Match::ShaderStage::eCompute, 0, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 1, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 2, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 3, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 4, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 5, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 6, Match::DescriptorType::eTexture },
        }).allocate()
            .bind_storage_buffer(0, resource.counts_buffer)
            .bind_storage_buffer(1, resource.visible_mesh_instance_indices_buffer)
            .bind_storage_buffer(2, resource.primitive_counts_buffer)
            .bind_storage_buffer(3, resource.indirect_commands_buffer)
            .bind_storage_buffer(4, resource.available_indirect_commands_buffer)
            .bind_storage_buffer(5, resource.instance_datas_buffer);
        for (size_t in_flight_index = 0; in_flight_index < Match::setting.max_in_flight_frame; in_flight_index ++) {
            vk::DescriptorImageInfo image_info {};
            image_info.setImageLayout(vk::ImageLayout::eGeneral)
                .setImageView(resource.depth_texture_views[in_flight_index])
                .setSampler(resource.depth_sampler);
            vk::WriteDescriptorSet descriptor_write {};
            descriptor_write.setDstSet(descriptor_set->descriptor_sets[in_flight_index])
                .setDstBinding(6)
                .setDstArrayElement(0)
                .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                .setImageInfo(image_info);
            manager->device->device.updateDescriptorSets({ descriptor_write }, {});
        }
        
        constants = factory->create_push_constants(
            Match::ShaderStage::eCompute, 
            {
                { "mesh_instance_count", Match::ConstantType::eUint32 },
                { "primitive_count", Match::ConstantType::eUint32 },
                { "depth_texture_size", Match::ConstantType::eUint32x2 },
            }
        );
        glm::uvec2 size = { width, height };
        constants->push_constant("depth_texture_size", &size);
        
        pre_culling_shader_program = factory->create_compute_shader_program();
        pre_culling_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/pre_culling.comp", Match::ShaderStage::eCompute))
            .attach_descriptor_set(descriptor_set, 0)
            .attach_descriptor_set(readonly_descriptor_set, 1)
            .attach_push_constants(constants)
            .compile();

        reset_indirect_command_shader_program = factory->create_compute_shader_program();
        reset_indirect_command_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/reset_indirect_command.comp", Match::ShaderStage::eCompute))
            .attach_descriptor_set(descriptor_set, 0)
            .attach_descriptor_set(readonly_descriptor_set, 1)
            .attach_push_constants(constants)
            .compile();
        reset_indirect_command_shader_program->descriptor_sets.clear();
        reset_indirect_command_shader_program->push_constants.reset();

        compact_instance_shader_program = factory->create_compute_shader_program();
        compact_instance_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/compact_instance.comp", Match::ShaderStage::eCompute))
            .attach_descriptor_set(descriptor_set, 0)
            .attach_descriptor_set(readonly_descriptor_set, 1)
            .attach_push_constants(constants)
            .compile();
        compact_instance_shader_program->descriptor_sets.clear();
        compact_instance_shader_program->push_constants.reset();

        generate_available_indirect_command_shader_program = factory->create_compute_shader_program();
        generate_available_indirect_command_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/generate_available_indirect_command.comp", Match::ShaderStage::eCompute))
            .attach_descriptor_set(descriptor_set, 0)
            .attach_descriptor_set(readonly_descriptor_set, 1)
            .attach_push_constants(constants)
            .compile();
        generate_available_indirect_command_shader_program->descriptor_sets.clear();
        generate_available_indirect_command_shader_program->push_constants.reset();
    }

    void CullingPass::executePreRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        // 执行剔除工作

        auto command_buffer = resource.culling_command_buffers.at(renderer->current_in_flight);
        command_buffer.reset();
        vk::CommandBufferBeginInfo begin_info {};
        command_buffer.begin(begin_info);

        // 1.将深度缓存复制到深度纹理的mip_level 0
        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, copy_depth_texture_shader_program->pipeline);
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, copy_depth_texture_shader_program->layout, 0, depth_descriptor_set->descriptor_sets[renderer->current_in_flight], {});
        auto [width, height] = Match::runtime_setting->get_window_size();
        glm::uvec2 image_size = { width, height };
        depth_constants->push_constant("image_size", &image_size);
        command_buffer.pushConstants(copy_depth_texture_shader_program->layout, vk::ShaderStageFlagBits::eCompute, 0, depth_constants->constants_size, depth_constants->constants.data());
        vk::ImageMemoryBarrier image_barrier {};
        image_barrier.setImage(renderer->framebuffer_set->attachments.at(renderer->render_pass_builder->get_attachment_index("depth", true)).image->image)
            .setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
            .setOldLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
            .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
            .setSubresourceRange({
                vk::ImageAspectFlagBits::eDepth | (Match::has_stencil_component(Match::get_supported_depth_format()) ? vk::ImageAspectFlagBits::eStencil : vk::ImageAspectFlagBits::eNone) ,
                0,
                1,
                0,
                1
            }); 
        command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eEarlyFragmentTests, vk::PipelineStageFlagBits::eComputeShader, {}, {}, {}, image_barrier);
        if (!global_runtime_context->render_system->getSwapData()->isCameraFixed()) {
            command_buffer.dispatch(std::ceil(image_size.x / 16.0), std::ceil(image_size.y / 16.0), 1);
        }
        image_barrier.setSrcAccessMask(vk::AccessFlagBits::eShaderRead)
            .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
            .setOldLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
            .setNewLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
        command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eEarlyFragmentTests, {}, {}, {}, image_barrier);

        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, generate_depth_texture_shader_program->pipeline);
        image_barrier.setImage(resource.depth_textures[renderer->current_in_flight]->image)
            .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
            .setOldLayout(vk::ImageLayout::eGeneral)
            .setNewLayout(vk::ImageLayout::eGeneral)
            .setSubresourceRange({
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            });
        command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eComputeShader, {}, {}, {}, image_barrier);
        // 2.根据深度纹理mip_level 0的数据生成后续层级的mipmap, 直到mipmap的图片大小为 1x? | ?x1
        for (uint32_t i = 1; i < resource.depth_mip_level_count; i ++) {
            glm::uvec2 image_size = { std::max<uint32_t>(1, width >> i), std::max<uint32_t>(1, height >> i) };
            depth_constants->push_constant("image_size", &image_size);
            depth_constants->push_constant("mip_level", i);
            command_buffer.pushConstants(copy_depth_texture_shader_program->layout, vk::ShaderStageFlagBits::eCompute, 0, depth_constants->constants_size, depth_constants->constants.data());
            command_buffer.dispatch(std::ceil(image_size.x / 16.0), std::ceil(image_size.y / 16.0), 1);
            image_barrier.setImage(resource.depth_textures[renderer->current_in_flight]->image)
                .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
                .setOldLayout(vk::ImageLayout::eGeneral)
                .setNewLayout(vk::ImageLayout::eGeneral)
                .setSubresourceRange({
                    vk::ImageAspectFlagBits::eColor,
                    i,
                    1,
                    0,
                    1
                });
            command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eComputeShader, {}, {}, {}, image_barrier);
        }

        // 3.遍历所有MeshInstance, 将所有不被剔除的实例收集起来
        uint32_t mesh_instance_count = global_runtime_context->render_system->getSwapData()->getMeshInstancePool()->current_instance_count;
        uint32_t primitive_count = global_runtime_context->assets_system->getMeshPool()->getPrimitiveCount();
        constants->push_constant("mesh_instance_count", mesh_instance_count);
        constants->push_constant("primitive_count", primitive_count);
        auto count_ptr = static_cast<uint32_t *>(resource.counts_buffer->in_flight_buffers[renderer->current_in_flight]->map());
        resource.visibility_count = count_ptr[3];
        resource.draw_call_count = count_ptr[4];
        count_ptr[0] = 0;
        count_ptr[3] = 0;
        count_ptr[4] = 0;

        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, pre_culling_shader_program->pipeline);
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, pre_culling_shader_program->layout, 0, { descriptor_set->descriptor_sets[renderer->current_in_flight], readonly_descriptor_set->descriptor_sets[renderer->current_in_flight] }, {});
        command_buffer.pushConstants(pre_culling_shader_program->layout, vk::ShaderStageFlagBits::eCompute, 0,  constants->constants_size, constants->constants.data());

        command_buffer.dispatch(std::ceil(mesh_instance_count / 256.0), 1, 1);

        vk::BufferMemoryBarrier barrier {};
        barrier.setBuffer(resource.primitive_counts_buffer->get_buffer(renderer->current_in_flight))
            .setOffset(0)
            .setSize(resource.primitive_counts_buffer->get_size())
            .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        // 4.重置IndirectCommand模板
        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, reset_indirect_command_shader_program->pipeline);
        command_buffer.dispatch(std::ceil(primitive_count / 256.0), 1, 1);

        barrier.setBuffer(resource.counts_buffer->get_buffer(renderer->current_in_flight))
            .setSize(resource.counts_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eShaderRead);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eDrawIndirect | vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        barrier.setBuffer(resource.visible_mesh_instance_indices_buffer->get_buffer(renderer->current_in_flight))
            .setSize(resource.visible_mesh_instance_indices_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        barrier.setBuffer(resource.indirect_commands_buffer->get_buffer(renderer->current_in_flight))
            .setSize(resource.indirect_commands_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        // 5.将所有可见的MeshInstance的数据按照批次储存在一起
        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, compact_instance_shader_program->pipeline);
        command_buffer.dispatchIndirect(resource.counts_buffer->get_buffer(renderer->current_in_flight), 0);

        barrier.setBuffer(resource.counts_buffer->get_buffer(renderer->current_in_flight))
            .setSize(resource.counts_buffer->get_size())
            .setSrcAccessMask(vk::AccessFlagBits::eShaderRead)
            .setDstAccessMask(vk::AccessFlagBits::eShaderWrite);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        barrier.setBuffer(resource.primitive_counts_buffer->get_buffer(renderer->current_in_flight))
            .setSize(resource.primitive_counts_buffer->get_size());
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderRead);
            // .setDstAccessMask(vk::AccessFlagBits::eShaderWrite);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        // 6.生成最终的IndirectCommand, 从模板IndirectCommand中剔除不绘制任何实例的IndirectCommand
        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, generate_available_indirect_command_shader_program->pipeline);
        command_buffer.dispatch(std::ceil(primitive_count / 256.0), 1, 1);

        command_buffer.end();
        vk::SubmitInfo submit_info {};
        submit_info.setCommandBuffers(command_buffer)
            .setSignalSemaphores(resource.culling_finish_semaphores.at(renderer->current_in_flight));
        resource.current_in_flight_wait_semaphore.push_back(resource.culling_finish_semaphores.at(renderer->current_in_flight));
        resource.current_in_flight_wait_stages.push_back(vk::PipelineStageFlagBits::eComputeShader);

        global_runtime_context->render_system->getMatchAPIManager()->device->compute_queue.submit(submit_info);
    }
}
