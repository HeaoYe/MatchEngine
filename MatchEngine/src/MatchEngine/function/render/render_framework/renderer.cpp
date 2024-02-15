#include <MatchEngine/function/render/render_framework/renderer.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    Renderer::Renderer() {
        auto builder = global_runtime_context->render_system->getMatchFactory()->create_render_pass_builder();
        if (output_attachment_name != Match::SWAPCHAIN_IMAGE_ATTACHMENT) {
            builder->add_attachment(output_attachment_name, Match::AttachmentType::eColorBuffer);
        }
        resource = std::make_unique<Resource>();

        subpasses.push_back(std::make_unique<MeshPass>());

        for (auto &subpass : subpasses) {
            subpass->createRenderResource(*builder, *resource);
        }
        for (auto &subpass : subpasses) {
            subpass->buildPassDescriptor(builder->add_subpass(subpass->getName()));
        }

        renderer = global_runtime_context->render_system->getMatchFactory()->create_renderer(builder);

        for (auto &subpass : subpasses) {
            subpass->postCreateRenderResource(renderer, *resource);
        }
    }
    
    Renderer::~Renderer() {
        renderer->wait_for_destroy();
        renderer.reset();
        subpasses.clear();
        resource.reset();
    }
    
    void Renderer::render() {
        renderer->acquire_next_image();
        for (auto &subpass : subpasses) {
            subpass->executePreRenderPass(renderer, *resource);
        }
        renderer->begin_render_pass();
        for (auto &subpass : subpasses) {
            subpass->executeRenderPass(renderer, *resource);
        }
        renderer->end_render_pass();
        for (auto &subpass : subpasses) {
            subpass->executePostRenderPass(renderer, *resource);
        }
        renderer->present();
    }
}
