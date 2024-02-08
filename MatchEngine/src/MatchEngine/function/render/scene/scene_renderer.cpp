#include <MatchEngine/function/render/scene/scene_renderer.hpp>
#include <Match/Match.hpp>
#include "internal.hpp"

namespace MatchEngine {
    SceneRenderer::SceneRenderer(const std::string &name) : name(name) {
        auto factory = global_runtime_context->render_system->getMatchFactory();

        auto render_pass_builder = factory->create_render_pass_builder();
        render_pass_builder->add_attachment("depth", Match::AttachmentType::eDepth);
        render_pass_builder->add_subpass("mesh pass")
            .attach_depth_attachment("depth")
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
        render_pass_builder->add_subpass("output pass")
            .attach_output_attachment(Match::SWAPCHAIN_IMAGE_ATTACHMENT)
            .wait_for(
                "mesh pass",
                {
                    .stage = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                    .access = vk::AccessFlagBits::eColorAttachmentRead
                },
                {
                    .stage = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                    .access = vk::AccessFlagBits::eColorAttachmentWrite
                });


        renderer = factory->create_renderer(render_pass_builder);
        renderer->set_clear_value(Match::SWAPCHAIN_IMAGE_ATTACHMENT, { { 0.3f, 0.3f, 0.3f, 1.0f } });

        mesh_pool = std::make_unique<MeshPool>(128 * 1024 * 1024, 128 * 1024 * 1024);
        render_target_pool = std::make_unique<RenderTargetPool>(Match::runtime_setting->get_window_size().width, Match::runtime_setting->get_window_size().height);

        for (size_t i = 0; i < Match::setting.max_in_flight_frame; i ++) {
            draw_call_queues.push_back(std::make_unique<SceneDrawCallQueue>(this));
        }
    }

    SceneRenderer::~SceneRenderer() {
        renderer->wait_for_destroy();
        draw_call_queues.clear();
        output_shader_program.reset();
        mesh_shader_program.reset();
        render_target_pool.reset();
        mesh_pool.reset();
        renderer.reset();
    }

    void SceneRenderer::addDrawCall(Game::CameraComponent *camera, RenderTargetID render_target_id) {
        draw_call_queues[renderer->current_in_flight]->addDrawCallData(camera, render_target_id);
    }
    
    void SceneRenderer::start() {
        mesh_pool->build();
        render_target_pool->build();

        auto factory = global_runtime_context->render_system->getMatchFactory();
        std::vector<Match::InputBindingInfo> binding_infos;
        mesh_pool->buildInputBindings(binding_infos);
        auto vas = factory->create_vertex_attribute_set(binding_infos);

        mesh_shader_program_decriptor_set = factory->create_descriptor_set();
        mesh_shader_program_decriptor_set->add_descriptors({
            { Match::ShaderStage::eVertex | Match::ShaderStage::eFragment, 0, Match::DescriptorType::eUniform },
        }).allocate();
        draw_call_data_uniform_buffer = factory->create_uniform_buffer(sizeof(SceneDrawCallQueue::DrawCallData));
        mesh_shader_program_decriptor_set->bind_uniform(0, draw_call_data_uniform_buffer);

        auto vert_shader = factory->compile_shader("mesh pass/shader.vert", Match::ShaderStage::eVertex);
        auto frag_shader = factory->compile_shader("mesh pass/shader.frag", Match::ShaderStage::eFragment);
        mesh_shader_program = factory->create_shader_program(renderer, "mesh pass");
        mesh_shader_program->attach_vertex_shader(vert_shader)
            .attach_fragment_shader(frag_shader)
            .attach_vertex_attribute_set(vas)
            .attach_descriptor_set(render_target_pool->render_target_descriptor_set, 0)
            .attach_descriptor_set(mesh_shader_program_decriptor_set, 1)
            .compile({
                .cull_mode = Match::CullMode::eBack,
                .front_face = Match::FrontFace::eCounterClockwise,
                .depth_test_enable = VK_TRUE,
            });
        vert_shader.reset();
        frag_shader.reset();
        
        vert_shader = factory->compile_shader("output pass/shader.vert", Match::ShaderStage::eVertex);
        frag_shader = factory->compile_shader("output pass/shader.frag", Match::ShaderStage::eFragment);
        output_shader_program = factory->create_shader_program(renderer, "output pass");
        output_shader_program->attach_vertex_shader(vert_shader)
            .attach_fragment_shader(frag_shader)
            .attach_descriptor_set(render_target_pool->render_target_descriptor_set)
            .compile({
                .cull_mode = Match::CullMode::eNone,
                .depth_test_enable = VK_FALSE,
            });
        vert_shader.reset();
        frag_shader.reset();
    }

    void SceneRenderer::render() {
        renderer->begin_render();

        mesh_pool->bind(renderer);
        renderer->bind_shader_program(mesh_shader_program);
        
        auto *queue = draw_call_queues.at(renderer->current_in_flight).get();
        while (!queue->empty()) {
            auto data = queue->fetchDrawCallData();
            memcpy(draw_call_data_uniform_buffer->get_uniform_ptr(), &data, sizeof(SceneDrawCallQueue::DrawCallData));

            render_target_pool->clearRenderTarget(data.render_target_id);
            renderer->get_command_buffer().drawIndexedIndirect(
                mesh_pool->indirect_command_buffer->get_buffer(renderer->current_in_flight),
                0, mesh_pool->indirect_command_count, sizeof(vk::DrawIndexedIndirectCommand)
            );
        }
        
        renderer->next_subpass();
        renderer->bind_shader_program(output_shader_program);
        renderer->draw(3, 1, 0, 0);

        renderer->end_render();
    }
}
