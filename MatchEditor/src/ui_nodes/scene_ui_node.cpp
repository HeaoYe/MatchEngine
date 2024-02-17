#include <ui_nodes/scene_ui_node.hpp>
#include <backends/imgui_impl_vulkan.h>

namespace MatchEditor {
    SceneUINode::SceneUINode() {
        sampler = MatchEngine::global_runtime_context->render_system->getMatchFactory()->create_sampler();
        scene_focus = false;
        scene_hover = false;

        MatchEngine::global_runtime_context->event_system->attachEventLayer(100, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::KeyPressedEvent>([&](auto &event) {
            return (!scene_focus) && (!scene_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MousePressedEvent>([&](auto &event) {
            if (event.button == MatchEngine::MouseButton::eMiddle) {
                auto x = MatchEngine::global_runtime_context->input_system->getMousePosX() - cursor_screen_pos.x;
                auto y = MatchEngine::global_runtime_context->input_system->getMousePosY() - cursor_screen_pos.y;
                if ((x >= 0) && (x < frame_extent.x) && (y >= 0) && (y < frame_extent.y)) {
                    float x_factor = static_cast<float>(x) / static_cast<float>(frame_extent.x);
                    float y_factor = static_cast<float>(y) / static_cast<float>(frame_extent.y);
                    x = x_factor * Match::runtime_setting->window_size.width;
                    y = y_factor * Match::runtime_setting->window_size.height;
                    MatchEngine::global_runtime_context->render_system->getRenderer()->pickGameObject(x, y, [](MatchEngine::Game::GameObjectUUID uuid) {
                        global_ui_context->is_selected[global_ui_context->selected_game_object_uuid] = false;
                        global_ui_context->is_selected[uuid] = true;
                        global_ui_context->selected_game_object_uuid = uuid;
                        MatchEngine::global_runtime_context->render_system->getRenderer()->reportSelectedGameObject(uuid);
                    }, []() {
                        global_ui_context->is_selected[global_ui_context->selected_game_object_uuid] = false;
                        global_ui_context->selected_game_object_uuid = MatchEngine::Game::GameObjectUUID(-1);
                        MatchEngine::global_runtime_context->render_system->getRenderer()->reportSelectedGameObject();
                    });
                }
            }
            return (!scene_focus) && (!scene_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MouseMovedEvent>([&](auto &event) {
            return (!scene_focus) && (!scene_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MouseScrollEvent>([&](auto &event) {
            return (!scene_focus) && (!scene_hover);
        }, "ImGui Event Block");

        auto renderer = MatchEngine::global_runtime_context->render_system->getRenderer()->renderer;
        callback_id = renderer->register_resource_recreate_callback([this]() {
            for (auto texture : in_flight_textures) {
                ImGui_ImplVulkan_RemoveTexture(texture);
            }
            in_flight_textures.clear();

            auto renderer = MatchEngine::global_runtime_context->render_system->getRenderer()->renderer;
            auto idx = renderer->render_pass_builder->get_attachment_index(MatchEngine::global_runtime_context->render_system->getOutputAttachmentName(), true);
            auto &attachment_info = renderer->render_pass_builder->attachments.at(idx);
            auto layout = attachment_info.description_write.finalLayout;
            if (attachment_info.description_read.has_value()) {
                layout = attachment_info.description_read->finalLayout;
            }
            
            for (size_t in_flight_index = 0; in_flight_index < Match::setting.max_in_flight_frame; in_flight_index ++) {
                in_flight_textures.push_back(ImGui_ImplVulkan_AddTexture(
                    sampler->sampler,
                    renderer->framebuffer_set->attachments.at(idx).image_view,
                    static_cast<VkImageLayout>(layout)
                ));
            }
        });
        renderer->callbacks.at(callback_id)();
    }

    SceneUINode::~SceneUINode() {
        auto renderer = MatchEngine::global_runtime_context->render_system->getRenderer()->renderer;
        renderer->remove_resource_recreate_callback(callback_id);

        for (auto texture : in_flight_textures) {
            ImGui_ImplVulkan_RemoveTexture(texture);
        }
    }

    void SceneUINode::render() {
        ImGui::Begin("Scene");
        scene_focus = ImGui::IsWindowFocused();
        scene_hover = ImGui::IsWindowHovered();
        cursor_screen_pos = ImGui::GetCursorScreenPos();
        frame_extent = ImGui::GetContentRegionAvail();
        // float expected_width = extent.y * camera->get_extent().x / camera->get_extent().y;
        // float pad = (expected_width - extent.x) / expected_width / 2.0f;
        ImGui::Image(in_flight_textures[Match::runtime_setting->current_in_flight], frame_extent);
        ImGui::End();
    }
}
