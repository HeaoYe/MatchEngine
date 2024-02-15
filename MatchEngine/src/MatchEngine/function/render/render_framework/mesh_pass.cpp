#include <MatchEngine/function/render/render_framework/mesh_pass.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    MeshPass::~MeshPass() {
        for (auto fence : compute_fences) {
            global_runtime_context->render_system->getMatchAPIManager()->device->device.destroyFence(fence);
        }
    }

    void MeshPass::createRenderResource(Match::RenderPassBuilder &builder, Resource &resource) {
        builder.add_attachment("depth", Match::AttachmentType::eDepth);
    }

    void MeshPass::postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto manager = global_runtime_context->render_system->getMatchAPIManager();
        auto factory = global_runtime_context->render_system->getMatchFactory();
        auto max_primitive_count = global_runtime_context->assets_system->getMaxPrimitiveCount();
        auto max_mesh_instance_count = global_runtime_context->render_system->getMaxMeshInstanceCount();

        resource.counts_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * 5, vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_AUTO, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        resource.primitive_counts_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * max_primitive_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.visible_mesh_instance_indices_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * 2 * max_mesh_instance_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.indirect_commands_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(vk::DrawIndexedIndirectCommand) * max_primitive_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.available_indirect_commands_buffer = std::make_shared<Match::InFlightBuffer>(resource.indirect_commands_buffer->get_size(), vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.instance_locations_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(glm::vec4) * max_mesh_instance_count, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.instance_rotations_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(glm::vec4) * max_mesh_instance_count, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        resource.instance_scales_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(glm::vec4) * max_mesh_instance_count, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

        vk::FenceCreateInfo fci {};
        command_buffers = manager->command_pool->allocate_command_buffer(Match::setting.max_in_flight_frame);
        for (auto &buffer : resource.counts_buffer->in_flight_buffers) {
            resource.counts_ptrs.push_back(static_cast<uint32_t *>(buffer->map()));
            resource.counts_ptrs.back()[0] = 1;
            resource.counts_ptrs.back()[1] = 1;
            resource.counts_ptrs.back()[2] = 1;
            resource.counts_ptrs.back()[3] = 0;
            resource.counts_ptrs.back()[4] = 0;
            compute_fences.push_back(manager->device->device.createFence(fci));
        }

        mesh_shader_program_descriptor_set = factory->create_descriptor_set();
        mesh_shader_program_descriptor_set->add_descriptors({
            { Match::ShaderStage::eVertex | Match::ShaderStage::eCompute, 0, Match::DescriptorType::eUniform },
            { Match::ShaderStage::eFragment, 1, Match::DescriptorType::eStorageBuffer }
        }).allocate()
            .bind_uniform(0, global_runtime_context->render_system->getSwapData()->getCameraUniformBuffer());

        compute_shader_program_descriptor_set = factory->create_descriptor_set();
        compute_shader_program_descriptor_set->add_descriptors({
            { Match::ShaderStage::eCompute, 0, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 1, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 2, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 3, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 4, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 5, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 6, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 7, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 8, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 9, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 10, Match::DescriptorType::eStorageBuffer },
        }).allocate()
            .bind_storage_buffer(0, resource.counts_buffer)
            .bind_storage_buffer(1, resource.primitive_counts_buffer)
            .bind_storage_buffer(2, global_runtime_context->render_system->getSwapData()->getMeshInstancePool()->mesh_instance_buffer)
            .bind_storage_buffer(3, resource.visible_mesh_instance_indices_buffer)
            .bind_storage_buffer(4, global_runtime_context->assets_system->getMeshPool()->mesh_descriptor_buffer)
            .bind_storage_buffer(5, global_runtime_context->assets_system->getMeshPool()->primitive_descriptor_buffer)
            .bind_storage_buffer(6, resource.indirect_commands_buffer)
            .bind_storage_buffer(7, resource.available_indirect_commands_buffer)
            .bind_storage_buffer(8, resource.instance_locations_buffer)
            .bind_storage_buffer(9, resource.instance_rotations_buffer)
            .bind_storage_buffer(10, resource.instance_scales_buffer);

        compute_shader_constants = factory->create_push_constants(Match::ShaderStage::eCompute, {
            { "mesh_instance_count", Match::ConstantType::eUint32 },
            { "primitive_count", Match::ConstantType::eUint32 }
        });

        auto collect_mesh_instance_shader = factory->compile_shader("mesh pass/collect_mesh_instance.comp", Match::ShaderStage::eCompute);
        collect_mesh_instance_shader_program = factory->create_compute_shader_program();
        collect_mesh_instance_shader_program->attach_compute_shader(collect_mesh_instance_shader)
            .attach_descriptor_set(compute_shader_program_descriptor_set, 0)
            .attach_descriptor_set(mesh_shader_program_descriptor_set, 1)
            .attach_push_constants(compute_shader_constants)
            .compile();
        
        auto generate_indirect_command_shader = factory->compile_shader("mesh pass/generate_indirect_command.comp", Match::ShaderStage::eCompute);
        generate_indirect_command_shader_program = factory->create_compute_shader_program();
        generate_indirect_command_shader_program->attach_compute_shader(generate_indirect_command_shader)
            .attach_descriptor_set(compute_shader_program_descriptor_set)
            .attach_push_constants(compute_shader_constants)
            .compile();
        generate_indirect_command_shader_program->descriptor_sets.clear();
        generate_indirect_command_shader_program->push_constants.reset();
        
        auto compact_instance_data_shader = factory->compile_shader("mesh pass/compact_instance_data.comp", Match::ShaderStage::eCompute);
        compact_instance_data_shader_program = factory->create_compute_shader_program();
        compact_instance_data_shader_program->attach_compute_shader(compact_instance_data_shader)
            .attach_descriptor_set(compute_shader_program_descriptor_set)
            .attach_push_constants(compute_shader_constants)
            .compile();
        compact_instance_data_shader_program->descriptor_sets.clear();
        compact_instance_data_shader_program->push_constants.reset();
        
        auto collect_available_indirect_command_shader = factory->compile_shader("mesh pass/collect_available_indirect_command.comp", Match::ShaderStage::eCompute);
        collect_available_indirect_command_shader_program = factory->create_compute_shader_program();
        collect_available_indirect_command_shader_program->attach_compute_shader(collect_available_indirect_command_shader)
            .attach_descriptor_set(compute_shader_program_descriptor_set)
            .attach_push_constants(compute_shader_constants)
            .compile();
        collect_available_indirect_command_shader_program->descriptor_sets.clear();
        collect_available_indirect_command_shader_program->push_constants.reset();

        auto vert_shader = factory->compile_shader("mesh pass/shader.vert", Match::ShaderStage::eVertex);
        auto frag_pre_z_shader = factory->compile_shader("mesh pass/shader_pre_z.frag", Match::ShaderStage::eFragment);
        auto frag_shader = factory->compile_shader("mesh pass/shader.frag", Match::ShaderStage::eFragment);
        auto vas = factory->create_vertex_attribute_set({
            { 0, Match::InputRate::ePerVertex, { Match::VertexType::eFloat3 } },
            { 1, Match::InputRate::ePerVertex, { Match::VertexType::eFloat3 } },
            { 2, Match::InputRate::ePerVertex, { Match::VertexType::eFloat2 } },
            { 3, Match::InputRate::ePerVertex, { Match::VertexType::eFloat3 } },
            { 4, Match::InputRate::ePerInstance, { Match::VertexType::eFloat4 } },
            { 5, Match::InputRate::ePerInstance, { Match::VertexType::eFloat4 } },
            { 6, Match::InputRate::ePerInstance, { Match::VertexType::eFloat4 } },
        });
        mesh_pre_z_shader_program = factory->create_shader_program(renderer, name);
        mesh_pre_z_shader_program->attach_vertex_shader(vert_shader)
            .attach_fragment_shader(frag_pre_z_shader)
            .attach_vertex_attribute_set(vas)
            .attach_descriptor_set(mesh_shader_program_descriptor_set)
            .compile({
                .cull_mode = Match::CullMode::eBack,
                .front_face = Match::FrontFace::eCounterClockwise,
                .depth_test_enable = VK_TRUE,
            });
        mesh_shader_program = factory->create_shader_program(renderer, name);
        mesh_shader_program->attach_vertex_shader(vert_shader)
            .attach_fragment_shader(frag_shader)
            .attach_vertex_attribute_set(vas)
            .attach_descriptor_set(mesh_shader_program_descriptor_set)
            .compile({
                .cull_mode = Match::CullMode::eBack,
                .front_face = Match::FrontFace::eCounterClockwise,
                .depth_test_enable = VK_TRUE,
                .depth_write_enable = VK_FALSE,
                .depth_compere_op = vk::CompareOp::eEqual,
            });
    }

    void MeshPass::buildPassDescriptor(Match::SubpassBuilder &builder) {
        builder.attach_depth_attachment("depth")
            .attach_output_attachment(output_attachment_name)
            .wait_for(
                Match::EXTERNAL_SUBPASS,
                {
                    .stage = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                    .access = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                },
                {
                    .stage = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                    .access = vk::AccessFlagBits::eNone,
                }
            );
    }

    void MeshPass::executePreRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        uint32_t mesh_instance_count = global_runtime_context->render_system->getSwapData()->getMeshInstancePool()->current_instance_count;
        uint32_t primitive_count = global_runtime_context->assets_system->getMeshPool()->getPrimitiveCount();
        compute_shader_constants->push_constant("mesh_instance_count", mesh_instance_count);
        compute_shader_constants->push_constant("primitive_count", primitive_count);
        auto command_buffer = command_buffers[renderer->current_in_flight];
        auto temp = renderer->current_buffer;
        renderer->current_buffer = command_buffer;

        command_buffer.reset();
        vk::CommandBufferBeginInfo begin_info {};
        command_buffer.begin(begin_info);
        resource.counts_ptrs[renderer->current_in_flight][0] = 1;
        resource.counts_ptrs[renderer->current_in_flight][3] = 0;
        resource.counts_ptrs[renderer->current_in_flight][4] = 0;

        renderer->bind_shader_program(collect_mesh_instance_shader_program);
        renderer->dispatch(std::ceil(mesh_instance_count / 64.0f));

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

        renderer->bind_shader_program(generate_indirect_command_shader_program);
        renderer->dispatch(std::ceil(primitive_count / 64.0f));

        barrier.setBuffer(resource.counts_buffer->get_buffer(renderer->current_in_flight))
            .setSize(resource.counts_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eShaderRead);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eAllCommands | vk::PipelineStageFlagBits::eAllGraphics,
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

        renderer->bind_shader_program(compact_instance_data_shader_program);
        command_buffer.dispatchIndirect(resource.counts_buffer->get_buffer(renderer->current_in_flight), 0);

        barrier.setBuffer(resource.counts_buffer->get_buffer(renderer->current_in_flight))
            .setSize(resource.counts_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderWrite);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eAllCommands | vk::PipelineStageFlagBits::eAllGraphics,
            {}, {}, barrier, {}
        );
        
        renderer->bind_shader_program(collect_available_indirect_command_shader_program);
        renderer->dispatch(std::ceil(primitive_count / 64.0f));

        command_buffer.end();

        global_runtime_context->render_system->getMatchAPIManager()->device->device.resetFences(compute_fences[renderer->current_in_flight]);
        vk::SubmitInfo submit_info {};
        submit_info.setCommandBuffers(command_buffer);
        global_runtime_context->render_system->getMatchAPIManager()->device->compute_queue.submit(submit_info, compute_fences[renderer->current_in_flight]);

        renderer->current_buffer = temp;
    }
    
    void MeshPass::executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        vk_check(global_runtime_context->render_system->getMatchAPIManager()->device->device.waitForFences(compute_fences[renderer->current_in_flight], VK_TRUE, std::numeric_limits<uint64_t>::max()));
        auto command_buffer = renderer->get_command_buffer();

        command_buffer.bindVertexBuffers(0, {
            global_runtime_context->assets_system->getMeshPool()->position_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->normal_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->tex_coord_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->color_buffer->buffer->buffer,
            resource.instance_locations_buffer->get_buffer(renderer->current_in_flight),
            resource.instance_rotations_buffer->get_buffer(renderer->current_in_flight),
            resource.instance_scales_buffer->get_buffer(renderer->current_in_flight)
        }, { 0, 0, 0, 0, 0, 0, 0 });
        renderer->bind_index_buffer(global_runtime_context->assets_system->getMeshPool()->index_buffer);

        renderer->bind_shader_program(mesh_pre_z_shader_program);
        for (size_t i = 0; i < resource.counts_ptrs[renderer->current_in_flight][4]; i ++) {
            command_buffer.drawIndexedIndirect(resource.available_indirect_commands_buffer->get_buffer(renderer->current_in_flight), sizeof(vk::DrawIndexedIndirectCommand) * i, 1, sizeof(vk::DrawIndexedIndirectCommand));
        }

        renderer->bind_shader_program(mesh_shader_program);
        for (size_t i = 0; i < resource.counts_ptrs[renderer->current_in_flight][4]; i ++) {
            command_buffer.drawIndexedIndirect(resource.available_indirect_commands_buffer->get_buffer(renderer->current_in_flight), sizeof(vk::DrawIndexedIndirectCommand) * i, 1, sizeof(vk::DrawIndexedIndirectCommand));
        }
        // command_buffer.drawIndexedIndirect(resource.available_indirect_commands_buffer->get_buffer(renderer->current_in_flight), 0, counts_ptrs[renderer->current_in_flight][4], sizeof(vk::DrawIndexedIndirectCommand));
    }
}
