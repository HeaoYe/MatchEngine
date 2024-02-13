#include <ui_nodes/scene_ui_node.hpp>
#include <backends/imgui_impl_vulkan.h>

namespace MatchEditor {
    SceneUINode::SceneUINode() {
        sampler = MatchEngine::global_runtime_context->render_system->getMatchFactory()->create_sampler();
        scene_focus = false;
        scene_hover = false;

        MatchEngine::global_runtime_context->event_system->attachEventLayer(100, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::KeyPressedEvent>([&](auto &event) {
            return (!scene_focus) || (!scene_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MousePressedEvent>([&](auto &event) {
            return (!scene_focus) || (!scene_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MouseMovedEvent>([&](auto &event) {
            return (!scene_focus) || (!scene_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MouseScrollEvent>([&](auto &event) {
            return (!scene_focus) || (!scene_hover);
        }, "ImGui Event Block");
    }

    void SceneUINode::preSceneRendererStart() {
        auto renderer = MatchEngine::global_runtime_context->render_system->getActiveSceneRenderer()->renderer;

        callback_id = renderer->register_resource_recreate_callback([&]() {
            for (auto texture : in_flight_textures) {
                ImGui_ImplVulkan_RemoveTexture(texture);
            }
            in_flight_textures.clear();

            auto scene_renderer = MatchEngine::global_runtime_context->render_system->getActiveSceneRenderer();
            auto idx = scene_renderer->renderer->render_pass_builder->get_attachment_index(MatchEngine::global_runtime_context->render_system->getOutputAttachmentName(), true);
            auto &attachment_info = scene_renderer->renderer->render_pass_builder->attachments.at(idx);
            auto layout = attachment_info.description_write.finalLayout;
            if (attachment_info.description_read.has_value()) {
                layout = attachment_info.description_read->finalLayout;
            }
            
            for (size_t in_flight_index = 0; in_flight_index < Match::setting.max_in_flight_frame; in_flight_index ++) {
                in_flight_textures.push_back(ImGui_ImplVulkan_AddTexture(
                    sampler->sampler,
                    scene_renderer->renderer->framebuffer_set->attachments.at(idx).image_view,
                    static_cast<VkImageLayout>(layout)
                ));
            }
        });
        renderer->callbacks.at(callback_id)();
    }

    void SceneUINode::onUnloadScene() {
        auto renderer = MatchEngine::global_runtime_context->render_system->getActiveSceneRenderer()->renderer;
        renderer->remove_resource_recreate_callback(callback_id);

        for (auto texture : in_flight_textures) {
            ImGui_ImplVulkan_RemoveTexture(texture);
        }
    }

    void SceneUINode::render() {
        ImGui::Begin("Scene");
        scene_focus = ImGui::IsWindowFocused();
        scene_hover = ImGui::IsWindowHovered();
        auto extent = ImGui::GetContentRegionAvail();
        // float expected_width = extent.y * camera->get_extent().x / camera->get_extent().y;
        // float pad = (expected_width - extent.x) / expected_width / 2.0f;
        ImGui::Image(in_flight_textures[Match::runtime_setting->current_in_flight], extent);
        ImGui::End();
    }
}
