#include <ui_nodes/inspector/inspector_ui_node.hpp>

namespace MatchEditor {
    InspectorUINode::InspectorUINode() {
        component_ui_manager = std::make_unique<ComponentUIManager>();
    }

    void InspectorUINode::onUnloadScene() {
        component_ui_manager->clearCache();
    }

    void InspectorUINode::render() {
        ImGui::Begin("Inspector");

        if (global_ui_context->selected_game_object_uuid == MatchEngine::Game::GameObjectUUID(-1)) {
            ImGui::End();
            return;
        }

        auto *game_object = MatchEngine::global_runtime_context->scene_manager->active_scene->game_object_map.at(global_ui_context->selected_game_object_uuid);
        for (auto *component : game_object->getComponets()) {
            component_ui_manager->renderComponent(component);
        }

        ImGui::End();
    }
}
