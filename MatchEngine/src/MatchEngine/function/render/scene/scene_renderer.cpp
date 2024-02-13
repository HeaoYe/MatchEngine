#include <MatchEngine/function/render/scene/scene_renderer.hpp>
#include <MatchEngine/function/render/pass/mesh_pass.hpp>
#include "internal.hpp"

namespace MatchEngine {
    SceneRenderer::SceneRenderer() {
        auto factory = global_runtime_context->render_system->getMatchFactory();
        auto render_pass_builder = factory->create_render_pass_builder();
        if (render_pass_builder->attachments_map.find(global_runtime_context->render_system->getOutputAttachmentName()) == render_pass_builder->attachments_map.end()) {
            render_pass_builder->add_attachment(global_runtime_context->render_system->getOutputAttachmentName(), Match::AttachmentType::eFloat4Buffer);
        }

        // GPU Driven Pipeline 渲染Mesh的Subpass
        subpasses.push_back(std::make_unique<MeshPass>());

        for (auto &pass : subpasses) {
            pass->createAttachent(*render_pass_builder);
        }
        for (auto &pass : subpasses) {
            pass->buildPassDescriptor(render_pass_builder->add_subpass(pass->getName()));
        }

        renderer = factory->create_renderer(render_pass_builder);

        swap_data = std::make_unique<SceneSwapData>();

        pre_scene_renderer_start = [](std::shared_ptr<Match::Renderer> renderer) {};
    }

    SceneRenderer::~SceneRenderer() {
        renderer->wait_for_destroy();
        subpasses.clear();
        renderer.reset();
        swap_data.reset();
    }
    
    void SceneRenderer::start() {
        pre_scene_renderer_start(renderer);

        for (auto &pass : subpasses) {
            pass->start(renderer);
        }
    }

    void SceneRenderer::render() {
        renderer->acquire_next_image();

        for (auto &pass : subpasses) {
            pass->executePreRenderPass(renderer);
        }

        renderer->begin_render_pass();

        for (auto &pass : subpasses) {
            renderer->continue_subpass_to(pass->getName());
            pass->executeRenderPass(renderer);
        }

        renderer->end_render_pass();

        for (auto &pass : subpasses) {
            pass->executePostRenderPass(renderer);
        }

        renderer->present();
    }
}
