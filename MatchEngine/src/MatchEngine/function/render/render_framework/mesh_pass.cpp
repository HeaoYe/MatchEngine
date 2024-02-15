#include <MatchEngine/function/render/render_framework/mesh_pass.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    void MeshPass::postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto manager = global_runtime_context->render_system->getMatchAPIManager();
        auto factory = global_runtime_context->render_system->getMatchFactory();

        mesh_shader_program_descriptor_set = factory->create_descriptor_set();
        mesh_shader_program_descriptor_set->add_descriptors({
            { Match::ShaderStage::eVertex, 0, Match::DescriptorType::eUniform },
        }).allocate()
            .bind_uniform(0, global_runtime_context->render_system->getSwapData()->getSceneCameraUniformBuffer());

        auto vert_shader = factory->compile_shader("mesh pass/shader.vert", Match::ShaderStage::eVertex);
        auto frag_shader = factory->compile_shader("mesh pass/shader.frag", Match::ShaderStage::eFragment);
        auto vas = factory->create_vertex_attribute_set({
            { 0, Match::InputRate::ePerVertex, { Match::VertexType::eFloat3 } },
            { 1, Match::InputRate::ePerVertex, { Match::VertexType::eFloat3 } },
            { 2, Match::InputRate::ePerVertex, { Match::VertexType::eFloat2 } },
            { 3, Match::InputRate::ePerVertex, { Match::VertexType::eFloat3 } },
            { 4, Match::InputRate::ePerInstance, { Match::VertexType::eFloat4, Match::VertexType::eFloat4, Match::VertexType::eFloat4 } },
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
    
    void MeshPass::executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto command_buffer = renderer->get_command_buffer();
        auto max_mesh_count = global_runtime_context->assets_system->getMaxMeshCount();

        command_buffer.bindVertexBuffers(0, {
            global_runtime_context->assets_system->getMeshPool()->position_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->normal_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->tex_coord_buffer->buffer->buffer,
            global_runtime_context->assets_system->getMeshPool()->color_buffer->buffer->buffer,
            resource.instance_datas_buffer->get_buffer(renderer->current_in_flight),
        }, { 0, 0, 0, 0, 0 });
        renderer->bind_index_buffer(global_runtime_context->assets_system->getMeshPool()->index_buffer);

        renderer->bind_shader_program(mesh_shader_program);
        command_buffer.drawIndexedIndirectCount(resource.available_indirect_commands_buffer->get_buffer(renderer->current_in_flight), 0, resource.counts_buffer->get_buffer(renderer->current_in_flight), sizeof(uint32_t) * 4, max_mesh_count, sizeof(vk::DrawIndexedIndirectCommand));
    }
}
