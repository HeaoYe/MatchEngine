#include <MatchEngine/function/render/pass/mesh_pass.hpp>
#include <Match/constant.hpp>
#include "internal.hpp"

namespace MatchEngine {
    MeshPass::MeshPass() : Subpass("mesh pass") {}

    MeshPass::~MeshPass() {
        for (auto fence : compute_fences) {
            global_runtime_context->window_system->getAPIManager()->device->device.destroyFence(fence);
        }
    }

    void MeshPass::createAttachent(Match::RenderPassBuilder &builder) {
        builder.add_attachment("depth", Match::AttachmentType::eDepth);
    }

    void MeshPass::buildPassDescriptor(Match::SubpassBuilder &builder) {
        builder.attach_depth_attachment("depth")
            .attach_output_attachment(Match::SWAPCHAIN_IMAGE_ATTACHMENT)
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

    void MeshPass::start(std::shared_ptr<Match::Renderer> renderer) {
        auto factory = global_runtime_context->render_system->getMatchFactory();
        auto max_primitive_count = global_runtime_context->assets_system->getMaxPrimitiveCount();
        auto mesh_instance_count = global_runtime_context->render_system->getActiveSceneRenderer()->getSwapData()->getMeshInstancePool()->getMeshInstanceCount();
        mesh_instance_count += 256;

        counts_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * 4, vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_AUTO, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        primitive_counts_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * max_primitive_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        visible_mesh_instance_indices_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(uint32_t) * 2 * mesh_instance_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        indirect_commands_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(vk::DrawIndexedIndirectCommand) * max_primitive_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        available_indirect_commands_buffer = std::make_shared<Match::InFlightBuffer>(indirect_commands_buffer->get_size(), vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        instance_locations_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(glm::vec4) * mesh_instance_count, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        instance_rotations_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(glm::vec4) * mesh_instance_count, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
        instance_scales_buffer = std::make_shared<Match::InFlightBuffer>(sizeof(glm::vec4) * mesh_instance_count, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

        vk::FenceCreateInfo fci {};
        command_buffers = global_runtime_context->window_system->getAPIManager()->command_pool->allocate_command_buffer(Match::setting.max_in_flight_frame);
        for (auto &buffer : counts_buffer->in_flight_buffers) {
            counts_ptrs.push_back(static_cast<uint32_t *>(buffer->map()));
            counts_ptrs.back()[1] = 1;
            counts_ptrs.back()[2] = 1;
            compute_fences.push_back(global_runtime_context->window_system->getAPIManager()->device->device.createFence(fci));
        }

        mesh_shader_program_descriptor_set = factory->create_descriptor_set();
        mesh_shader_program_descriptor_set->add_descriptors({
            { Match::ShaderStage::eVertex | Match::ShaderStage::eCompute, 0, Match::DescriptorType::eUniform }
        }).allocate()
            .bind_uniform(0, global_runtime_context->render_system->getActiveSceneRenderer()->getSwapData()->getCameraUniformBuffer());

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
            .bind_storage_buffer(0, counts_buffer)
            .bind_storage_buffer(1, primitive_counts_buffer)
            .bind_storage_buffer(2, global_runtime_context->render_system->getActiveSceneRenderer()->getSwapData()->getMeshInstancePool()->mesh_instance_buffer)
            .bind_storage_buffer(3, visible_mesh_instance_indices_buffer)
            .bind_storage_buffer(4, global_runtime_context->assets_system->getMeshPool()->mesh_descriptor_buffer)
            .bind_storage_buffer(5, global_runtime_context->assets_system->getMeshPool()->primitive_descriptor_buffer)
            .bind_storage_buffer(6, indirect_commands_buffer)
            .bind_storage_buffer(7, available_indirect_commands_buffer)
            .bind_storage_buffer(8, instance_locations_buffer)
            .bind_storage_buffer(9, instance_rotations_buffer)
            .bind_storage_buffer(10, instance_scales_buffer);

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
        mesh_shader_program = factory->create_shader_program(renderer, name);
        mesh_shader_program->attach_vertex_shader(vert_shader)
            .attach_fragment_shader(frag_shader)
            .attach_vertex_attribute_set(vas)
            .attach_descriptor_set(mesh_shader_program_descriptor_set)
            .compile({
                .cull_mode = Match::CullMode::eBack,
                .front_face = Match::FrontFace::eCounterClockwise,
                .depth_test_enable = VK_TRUE,
            });
    }

    void MeshPass::executePreRenderPass(std::shared_ptr<Match::Renderer> renderer) {
        uint32_t mesh_instance_count = global_runtime_context->render_system->getActiveSceneRenderer()->getSwapData()->getMeshInstancePool()->getMeshInstanceCount();
        uint32_t primitive_count = global_runtime_context->assets_system->getMeshPool()->getPrimitiveCount();
        compute_shader_constants->push_constant("mesh_instance_count", mesh_instance_count);
        compute_shader_constants->push_constant("primitive_count", primitive_count);
        auto command_buffer = command_buffers[renderer->current_in_flight];
        auto temp = renderer->current_buffer;
        renderer->current_buffer = command_buffer;

        command_buffer.reset();
        vk::CommandBufferBeginInfo begin_info {};
        command_buffer.begin(begin_info);
        counts_ptrs[renderer->current_in_flight][0] = 0;
        counts_ptrs[renderer->current_in_flight][3] = 0;

        renderer->bind_shader_program(collect_mesh_instance_shader_program);
        renderer->dispatch(std::ceil(mesh_instance_count / 64.0f));

        vk::BufferMemoryBarrier barrier {};
        barrier.setBuffer(primitive_counts_buffer->get_buffer(renderer->current_in_flight))
            .setOffset(0)
            .setSize(primitive_counts_buffer->get_size())
            .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        renderer->bind_shader_program(generate_indirect_command_shader_program);
        renderer->dispatch(std::ceil(primitive_count / 64.0f));

        barrier.setBuffer(counts_buffer->get_buffer(renderer->current_in_flight))
            .setSize(counts_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eShaderRead);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eAllCommands | vk::PipelineStageFlagBits::eAllGraphics,
            {}, {}, barrier, {}
        );

        barrier.setBuffer(visible_mesh_instance_indices_buffer->get_buffer(renderer->current_in_flight))
            .setSize(visible_mesh_instance_indices_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        barrier.setBuffer(indirect_commands_buffer->get_buffer(renderer->current_in_flight))
            .setSize(indirect_commands_buffer->get_size())
            // .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite);
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eComputeShader,
            vk::PipelineStageFlagBits::eComputeShader,
            {}, {}, barrier, {}
        );

        renderer->bind_shader_program(compact_instance_data_shader_program);
        command_buffer.dispatchIndirect(counts_buffer->get_buffer(renderer->current_in_flight), 0);

        barrier.setBuffer(counts_buffer->get_buffer(renderer->current_in_flight))
            .setSize(counts_buffer->get_size())
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

        global_runtime_context->window_system->getAPIManager()->device->device.resetFences(compute_fences[renderer->current_in_flight]);
        vk::SubmitInfo submit_info {};
        submit_info.setCommandBuffers(command_buffer);
        global_runtime_context->window_system->getAPIManager()->device->compute_queue.submit(submit_info, compute_fences[renderer->current_in_flight]);

        renderer->current_buffer = temp;
    }
    
    void MeshPass::executeRenderPass(std::shared_ptr<Match::Renderer> renderer) {
        vk_check(global_runtime_context->window_system->getAPIManager()->device->device.waitForFences(compute_fences[renderer->current_in_flight], VK_TRUE, std::numeric_limits<uint64_t>::max()));
        auto command_buffer = renderer->get_command_buffer();

        command_buffer.bindVertexBuffers(0, {
            global_runtime_context->assets_system->getMeshPool()->position_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->normal_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->tex_coord_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->color_buffer->buffer->buffer,
            instance_locations_buffer->get_buffer(renderer->current_in_flight),
            instance_rotations_buffer->get_buffer(renderer->current_in_flight),
            instance_scales_buffer->get_buffer(renderer->current_in_flight)
        }, { 0, 0, 0, 0, 0, 0, 0 });
        renderer->bind_index_buffer(global_runtime_context->assets_system->getMeshPool()->index_buffer);

        renderer->bind_shader_program(mesh_shader_program);
        command_buffer.drawIndexedIndirect(available_indirect_commands_buffer->get_buffer(renderer->current_in_flight), 0, counts_ptrs[renderer->current_in_flight][3], sizeof(vk::DrawIndexedIndirectCommand));
    }
}
