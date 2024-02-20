#include <ui_nodes/viewport/viewport_ui_node.hpp>
#include <backends/imgui_impl_vulkan.h>
#include <MatchEngine/game_framework/component/camera/perspective_camera_component.hpp>
#include <MatchEngine/game_framework/component/transform/transform_component.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MatchEditor {
    ViewportUINode::ViewportUINode() {
        sampler = MatchEngine::global_runtime_context->render_system->getMatchFactory()->create_sampler();
        viewport_focus = false;
        viewport_hover = false;
        on_select_game_object_callback = [](MatchEngine::Game::GameObjectUUID uuid) {};
        camera = std::make_unique<ViewportCamera>();

        MatchEngine::global_runtime_context->event_system->attachEventLayer(100, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::KeyPressedEvent>([&](auto &event) {
            return (!viewport_focus) && (!viewport_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MousePressedEvent>([this](auto &event) {
            if (event.button == MatchEngine::MouseButton::eMiddle) {
                auto x = MatchEngine::global_runtime_context->input_system->getMousePosX() - cursor_screen_pos.x;
                auto y = MatchEngine::global_runtime_context->input_system->getMousePosY() - cursor_screen_pos.y;
                if ((x >= 0) && (x < frame_extent.x) && (y >= 0) && (y < frame_extent.y)) {
                    float x_factor = static_cast<float>(x) / static_cast<float>(frame_extent.x);
                    float y_factor = static_cast<float>(y) / static_cast<float>(frame_extent.y);
                    x = x_factor * Match::runtime_setting->window_size.width;
                    y = y_factor * Match::runtime_setting->window_size.height;
                    MatchEngine::global_runtime_context->render_system->getRenderer()->pickGameObject(x, y, [this](MatchEngine::Game::GameObjectUUID uuid) {
                        on_select_game_object_callback(uuid);
                    }, [this]() {
                        on_select_game_object_callback(MatchEngine::Game::GameObjectUUID(-1));
                    });
                }
            }
            return (!viewport_focus) && (!viewport_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MouseMovedEvent>([&](auto &event) {
            return (!viewport_focus) && (!viewport_hover);
        }, "ImGui Event Block");
        MatchEngine::global_runtime_context->event_system->addEventListener<MatchEngine::MouseScrollEvent>([&](auto &event) {
            return (!viewport_focus) && (!viewport_hover);
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

    ViewportUINode::~ViewportUINode() {
        auto renderer = MatchEngine::global_runtime_context->render_system->getRenderer()->renderer;
        renderer->remove_resource_recreate_callback(callback_id);

        for (auto texture : in_flight_textures) {
            ImGui_ImplVulkan_RemoveTexture(texture);
        }
    }

    void ViewportUINode::onLoadScene() {
        camera->reset();
        MatchEngine::global_runtime_context->camera_system->activeEditorCamera(global_ui_context->viewport_camera_id);
    }

    void ViewportUINode::onTick(float dt) {
        camera->onTick(dt);
    }

    void ViewportUINode::render() {
        ImGui::Begin("Viewport");
        viewport_focus = ImGui::IsWindowFocused();
        viewport_hover = ImGui::IsWindowHovered();
        cursor_screen_pos = ImGui::GetCursorScreenPos();
        frame_extent = ImGui::GetContentRegionAvail();
        // float expected_width = extent.y * camera->get_extent().x / camera->get_extent().y;
        // float pad = (expected_width - extent.x) / expected_width / 2.0f;
        ImGui::Image(in_flight_textures[Match::runtime_setting->current_in_flight], frame_extent);

        if (global_ui_context->mode == Mode::eEdit) {
            auto camera_data = MatchEngine::global_runtime_context->camera_system->quertyCameraData(global_ui_context->viewport_camera_id);
            if (global_ui_context->selected_game_object_uuid != MatchEngine::Game::GameObjectUUID(-1)) {
                auto *t = MatchEngine::global_runtime_context->scene_manager->active_scene->game_object_map.at(global_ui_context->selected_game_object_uuid)->queryComponent<MatchEngine::Game::TransformComponent>();
                if (t != nullptr) {
                    glm::mat4 mat;
                    ImGuizmo::SetOrthographic(false);
                    // ImGuizmo::set
                    ImGuizmo::SetDrawlist();
                    ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(t->location), glm::value_ptr(t->rotation), glm::value_ptr(t->scale), glm::value_ptr(mat));
                    ImGuizmo::SetRect(cursor_screen_pos.x, cursor_screen_pos.y, frame_extent.x, frame_extent.y);
                    ImGuizmo::Manipulate(glm::value_ptr(camera_data->view), glm::value_ptr(camera_data->project), ImGuizmo::OPERATION::UNIVERSAL, ImGuizmo::MODE::WORLD, glm::value_ptr(mat));
                    if (ImGuizmo::IsUsing()) {
                        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(mat), &t->location.x, &t->rotation.x, &t->scale.x);
                        t->onMemberUpdate();
                    }
                }
            }
        }

        ImGui::End();
    }
}
