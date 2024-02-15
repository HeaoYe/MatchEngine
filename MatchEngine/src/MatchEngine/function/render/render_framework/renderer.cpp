#include <MatchEngine/function/render/render_framework/renderer.hpp>
#include <MatchEngine/function/render/render_framework/culling_pass.hpp>
#include <MatchEngine/function/render/render_framework/mesh_pass.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    Renderer::Renderer() {
        auto builder = global_runtime_context->render_system->getMatchFactory()->create_render_pass_builder();
        if (output_attachment_name != Match::SWAPCHAIN_IMAGE_ATTACHMENT) {
            builder->add_attachment(output_attachment_name, Match::AttachmentType::eColorBuffer);
        }
        resource = std::make_unique<Resource>();

        subpasses.push_back(std::make_unique<CullingPass>());
        subpasses.push_back(std::make_unique<MeshPass>());

        for (auto &subpass : subpasses) {
            subpass->createRenderResource(*builder);
        }
        for (auto &subpass : subpasses) {
            subpass->buildPassDescriptor(builder->add_subpass(subpass->getName()));
        }

        renderer = global_runtime_context->render_system->getMatchFactory()->create_renderer(builder);
    }
    
    Renderer::~Renderer() {
        renderer->wait_for_destroy();
        renderer.reset();
        subpasses.clear();
        auto manager = global_runtime_context->render_system->getMatchAPIManager();
        manager->device->device.destroySampler(resource->depth_sampler);
        for (auto &views : resource->depth_texture_mipmap_views) {
            for (auto view : views) {
                manager->device->device.destroyImageView(view);
            }
        }
        for (auto view : resource->depth_texture_views) {
            manager->device->device.destroyImageView(view);
        }
        for (auto semaphore : resource->culling_finish_semaphores) {
            manager->device->device.destroySemaphore(semaphore);
        }
        resource.reset();
    }

    void Renderer::start() {
        for (auto &subpass : subpasses) {
            subpass->postCreateRenderResource(renderer, *resource);
        }
    }
    
    void Renderer::render() {
        renderer->acquire_next_image();
        for (auto &subpass : subpasses) {
            subpass->executePreRenderPass(renderer, *resource);
        }
        renderer->begin_render_pass();
        for (auto &subpass : subpasses) {
            renderer->continue_subpass_to(subpass->getName());
            subpass->executeRenderPass(renderer, *resource);
        }
        renderer->end_render_pass();
        for (auto &subpass : subpasses) {
            subpass->executePostRenderPass(renderer, *resource);
        }
        renderer->present(resource->current_in_flight_wait_stages, resource->current_in_flight_wait_semaphore);
        resource->current_in_flight_wait_stages.clear();
        resource->current_in_flight_wait_semaphore.clear();
    }
}
