#include <MatchEngine/function/render/render_framework/renderer.hpp>
#include <MatchEngine/function/render/render_framework/culling_pass.hpp>
#include <MatchEngine/function/render/render_framework/visibility_pass.hpp>
#include <MatchEngine/function/render/render_framework/mesh_pass.hpp>
#include <MatchEngine/function/render/render_framework/outlining_pass.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    Renderer::Renderer() {
        auto builder = global_runtime_context->render_system->getMatchFactory()->create_render_pass_builder();
        if (global_runtime_context->editor_mode) {
            global_runtime_context->render_system->output_attachment_name = "RendererOutput";
            builder->add_attachment(global_runtime_context->render_system->output_attachment_name, Match::AttachmentType::eColorBuffer);
        } else {
            global_runtime_context->render_system->output_attachment_name = Match::SWAPCHAIN_IMAGE_ATTACHMENT;
        }
        resource = std::make_unique<Resource>();
        resource->in_flight_wait_stages.resize(Match::setting.max_in_flight_frame);
        resource->in_flight_wait_semaphore.resize(Match::setting.max_in_flight_frame);

        subpasses.push_back(std::make_unique<CullingPass>());
        subpasses.push_back(std::make_unique<VisibilityPass>());
        subpasses.push_back(std::make_unique<MeshPass>());
        subpasses.push_back(std::make_unique<OutliningPass>());

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
        picker = std::make_unique<GameObjectPicker>(renderer, *resource);
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
        renderer->present(resource->in_flight_wait_stages[renderer->current_in_flight], resource->in_flight_wait_semaphore[renderer->current_in_flight]);
        resource->in_flight_wait_stages[renderer->current_in_flight].clear();
        resource->in_flight_wait_semaphore[renderer->current_in_flight].clear();
        if (picker->needProcess()) {
            auto in_flight_idnex = (Match::runtime_setting->current_in_flight == 0) ? (Match::setting.max_in_flight_frame - 1) : (Match::runtime_setting->current_in_flight - 1);
            vk_check(global_runtime_context->render_system->getMatchAPIManager()->device->device.waitForFences(renderer->in_flight_fences[in_flight_idnex], VK_TRUE, UINT64_MAX));
            picker->processPickTasks(renderer, in_flight_idnex);
        }
    }
 
    void Renderer::pickGameObject(uint32_t x, uint32_t y, std::function<void(Game::GameObjectUUID uuid)> picked_callback, std::function<void()> miss_callback) {
        return picker->addPickTask({
            .x = x,
            .y = y,
            .picked_callback = picked_callback,
            .miss_callback = miss_callback,
        });
    }
 
    void Renderer::reportSelectedGameObject(Game::GameObjectUUID uuid) {
        auto &map = global_runtime_context->render_system->getSwapData()->getMeshInstancePool()->game_object_uuid_to_mesh_instance_index_map;
        if (auto iter = map.find(uuid); iter != map.end()) {
            resource->selected_mesh_instance_index = iter->second;
        } else {
            resource->selected_mesh_instance_index = uint32_t(-1);
        }
    }
}
