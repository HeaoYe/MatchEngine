#include <MatchEngine/function/render/render_framework/outlining_pass.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    void OutliningPass::buildPassDescriptor(Match::SubpassBuilder &builder) {
        builder.attach_depth_attachment("depth")
            .attach_output_attachment(global_runtime_context->render_system->getOutputAttachmentName())
            .wait_for(
                "mesh pass",
                {
                    .stage = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                    .access = vk::AccessFlagBits::eColorAttachmentWrite,
                }, 
                {
                    .stage = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                    .access = vk::AccessFlagBits::eColorAttachmentWrite,
                });
    }

    void OutliningPass::postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto factory = global_runtime_context->render_system->getMatchFactory();

        auto vert_shader = factory->compile_shader(getName() + "/shader.vert", Match::ShaderStage::eVertex);
        auto stencil_frag_shader = factory->compile_shader(getName() + "/stencil.frag", Match::ShaderStage::eFragment);
        auto outlining_frag_shader = factory->compile_shader(getName() + "/outlining.frag", Match::ShaderStage::eFragment);
        auto vas = factory->create_vertex_attribute_set({
            { 0, Match::InputRate::ePerVertex, { Match::VertexType::eFloat3 } },
            { 1, Match::InputRate::ePerInstance, { Match::VertexType::eFloat4, Match::VertexType::eFloat4, Match::VertexType::eFloat4 } },
        });

        descriptor_set = factory->create_descriptor_set();
        descriptor_set->add_descriptors({
            { Match::ShaderStage::eVertex , 0, Match::DescriptorType::eUniform },
        }).allocate()
            .bind_uniform(0, global_runtime_context->render_system->getSwapData()->getSceneCameraUniformBuffer());

        stencil_shader_program = factory->create_shader_program(renderer, getName());
        stencil_shader_program->attach_vertex_shader(vert_shader)
            .attach_fragment_shader(stencil_frag_shader)
            .attach_vertex_attribute_set(vas)
            .attach_descriptor_set(descriptor_set)
            .compile({
                .cull_mode = Match::CullMode::eBack,
                .front_face = Match::FrontFace::eCounterClockwise,
                .depth_test_enable = VK_FALSE,
                .stencil_test_enable = VK_TRUE,
                .stencil_front = (vk::StencilOpState {}).setPassOp(vk::StencilOp::eReplace).setFailOp(vk::StencilOp::eReplace).setReference(1).setCompareOp(vk::CompareOp::eAlways).setCompareMask(0xff).setWriteMask(0xff),
            });

        outlining_shader_program = factory->create_shader_program(renderer, getName());
        outlining_shader_program->attach_vertex_shader(vert_shader)
            .attach_fragment_shader(outlining_frag_shader)
            .attach_vertex_attribute_set(vas)
            .attach_descriptor_set(descriptor_set)
            .compile({
                .polygon_mode = Match::PolygonMode::eLine,
                .line_width = 6.0f,
                .cull_mode = Match::CullMode::eBack,
                .front_face = Match::FrontFace::eCounterClockwise,
                .depth_test_enable = VK_FALSE,
                .stencil_test_enable = VK_TRUE,
                .stencil_front = (vk::StencilOpState {}).setPassOp(vk::StencilOp::eReplace).setFailOp(vk::StencilOp::eKeep).setReference(1).setCompareOp(vk::CompareOp::eNotEqual).setCompareMask(0xff).setWriteMask(0xff),
            });
        outlining_shader_program->descriptor_sets.clear();
    }

    void OutliningPass::executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        if (resource.selected_mesh_instance_index != uint32_t(-1)) {
            renderer->bind_shader_program(stencil_shader_program);
            auto command_buffer = renderer->get_command_buffer();
            command_buffer.bindVertexBuffers(1, resource.outlining_buffer->get_buffer(renderer->current_in_flight), { 0 });

            command_buffer.drawIndexedIndirect(resource.outlining_buffer->get_buffer(renderer->current_in_flight), sizeof(glm::vec4) * 3, 1, sizeof(vk::DrawIndexedIndirectCommand));
            renderer->bind_shader_program(outlining_shader_program);
            command_buffer.drawIndexedIndirect(resource.outlining_buffer->get_buffer(renderer->current_in_flight), sizeof(glm::vec4) * 3, 1, sizeof(vk::DrawIndexedIndirectCommand));
        }
    }
}
