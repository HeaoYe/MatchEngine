#include <MatchEngine/function/render/render_framework/light_pass.hpp>
#include <Match/core/setting.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    void LightPass::postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        // 将屏幕以32像素分为多个格子, 计算每个格子可见的点光源, 渲染时只需要渲染可见的电光源即可
        // 具体剔除方法为:
        //      遍历所有光源
        //          遍历所有格子
        //          计算格子的圆锥包围盒
        //          如果点光源与包围盒相交, 就把点光源信息写入缓存
        // 渲染:
        //      根据当前渲染的屏幕位置, 找到对应格子
        //      根据格子, 找到包含的点光源
        //      仅渲染包含的点光源
        resource.tile_pixel_size = 32;
        resource.tile_x = std::ceil(static_cast<float>(Match::runtime_setting->window_size.width) / static_cast<float>(resource.tile_pixel_size));
        resource.tile_y = std::ceil(static_cast<float>(Match::runtime_setting->window_size.height) / static_cast<float>(resource.tile_pixel_size));
        uint32_t tile_count = resource.tile_x * resource.tile_y;
        resource.tile_point_light_counts_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * tile_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.tile_point_light_indices_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * tile_count * global_runtime_context->render_system->getMaxPointLightCount(), vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.tile_view_z_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * 2 * tile_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

        auto factory = global_runtime_context->render_system->getMatchFactory();

        constants = factory->create_push_constants(
            Match::ShaderStage::eCompute,
            {
                { "image_size", Match::ConstantType::eUint32x2 },
                { "tile_size", Match::ConstantType::eUint32x2 },
                { "light_count", Match::ConstantType::eUint32 },
            });
        glm::uvec2 size = { resource.tile_x, resource.tile_y };
        constants->push_constant("tile_size", &size);

        descriptor_set = factory->create_descriptor_set();
        descriptor_set->add_descriptors({
            { Match::ShaderStage::eCompute, 0, Match::DescriptorType::eStorageImage },
            { Match::ShaderStage::eCompute, 1, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 2, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 3, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 4, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 5, Match::DescriptorType::eUniform },
        }).allocate()
            .bind_storage_buffer(1, resource.tile_view_z_buffer)
            .bind_storage_buffer(2, resource.tile_point_light_counts_buffer)
            .bind_storage_buffer(3, resource.tile_point_light_indices_buffer)
            .bind_storage_buffer(4, global_runtime_context->render_system->getSwapData()->getPointLightPool()->point_light_buffer)
            .bind_uniform(5, global_runtime_context->camera_system->getViewportCamera());
        for (size_t in_flight_index = 0; in_flight_index < Match::setting.max_in_flight_frame; in_flight_index ++) {
            vk::DescriptorImageInfo image_info {};
            image_info.setImageLayout(vk::ImageLayout::eGeneral)
                .setImageView(resource.depth_texture_mipmap_views[in_flight_index][0]);
            vk::WriteDescriptorSet descriptor_write {};
            descriptor_write.setDstSet(descriptor_set->descriptor_sets[in_flight_index])
                .setDstBinding(0)
                .setDstArrayElement(0)
                .setDescriptorType(vk::DescriptorType::eStorageImage)
                .setImageInfo(image_info);
            global_runtime_context->render_system->getMatchAPIManager()->device->device.updateDescriptorSets({ descriptor_write }, {});
        }

        clear_data_shader_program = factory->create_compute_shader_program();
        clear_data_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/clear_data.comp", Match::ShaderStage::eCompute))
            .attach_push_constants(constants)
            .attach_descriptor_set(descriptor_set)
            .compile();
        clear_data_shader_program->push_constants.reset();
        clear_data_shader_program->descriptor_sets.clear();

        compute_tile_depth_shader_program = factory->create_compute_shader_program();
        compute_tile_depth_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/compute_tile_depth.comp", Match::ShaderStage::eCompute))
            .attach_push_constants(constants)
            .attach_descriptor_set(descriptor_set)
            .compile();
        compute_tile_depth_shader_program->push_constants.reset();
        compute_tile_depth_shader_program->descriptor_sets.clear();

        compute_tile_point_light_shader_program = factory->create_compute_shader_program();
        compute_tile_point_light_shader_program->attach_compute_shader(factory->compile_shader(getName() + "/compute_tile_point_light.comp", Match::ShaderStage::eCompute))
            .attach_push_constants(constants)
            .attach_descriptor_set(descriptor_set)
            .compile();
        compute_tile_point_light_shader_program->push_constants.reset();
        compute_tile_point_light_shader_program->descriptor_sets.clear();
    }

    void LightPass::executePreRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto command_buffer = resource.culling_command_buffers.at(renderer->current_in_flight);

        auto [width, height] = Match::runtime_setting->window_size;
        uint32_t light_count = global_runtime_context->render_system->getSwapData()->getPointLightPool()->current_point_light_count;
        glm::uvec2 size = { width, height };

        constants->push_constant("image_size", &size);
        constants->push_constant("light_count", light_count);
        command_buffer.pushConstants(clear_data_shader_program->layout, vk::ShaderStageFlagBits::eCompute, 0, constants->constants_size, constants->constants.data());
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, clear_data_shader_program->layout, 0, descriptor_set->descriptor_sets[renderer->current_in_flight], {});
        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, clear_data_shader_program->pipeline);
        command_buffer.dispatch(resource.tile_x, resource.tile_y, 1);

        vk::BufferMemoryBarrier barrier {};
        barrier.setBuffer(resource.tile_view_z_buffer->get_buffer(renderer->current_in_flight))
            .setOffset(0)
            .setSize(resource.tile_view_z_buffer->get_size())
            .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderWrite);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, compute_tile_depth_shader_program->pipeline);
        command_buffer.dispatch(resource.tile_x, resource.tile_y, 1);

        barrier.setBuffer(resource.tile_view_z_buffer->get_buffer(renderer->current_in_flight))
            // .setSize(resource.tile_depth_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        barrier.setBuffer(resource.tile_point_light_counts_buffer->get_buffer(renderer->current_in_flight))
            .setSize(resource.tile_point_light_counts_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderWrite);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, compute_tile_point_light_shader_program->pipeline);
        command_buffer.dispatch(std::ceil(static_cast<float>(light_count) / 32.0), 1, 1);

        command_buffer.end();
        vk::SubmitInfo submit_info {};
        submit_info.setCommandBuffers(resource.culling_command_buffers.at(renderer->current_in_flight))
            .setSignalSemaphores(resource.culling_finish_semaphores.at(renderer->current_in_flight));
        resource.in_flight_wait_semaphore[renderer->current_in_flight].push_back(resource.culling_finish_semaphores.at(renderer->current_in_flight));
        resource.in_flight_wait_stages[renderer->current_in_flight].push_back(vk::PipelineStageFlagBits::eDrawIndirect);

        renderer->report_submit_info(submit_info);
    }
}
