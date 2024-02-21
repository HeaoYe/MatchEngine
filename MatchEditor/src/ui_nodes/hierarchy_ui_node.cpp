#include <ui_nodes/hierarchy_ui_node.hpp>

namespace MatchEditor {
    void HierarchyUINode::onLoadScene() {
        for (auto *go : MatchEngine::global_runtime_context->scene_manager->active_scene->game_objects) {
            ui_is_selected.insert(std::make_pair(go->getUUID(), false));
        }
        global_ui_context->selected_game_object_uuid = MatchEngine::Game::GameObjectUUID(-1);
    }

    void HierarchyUINode::onUnloadScene() {
        ui_is_selected.clear();
    }

    void HierarchyUINode::render() {
        ImGui::Begin("Hierarchy");
        for (auto *go : MatchEngine::global_runtime_context->scene_manager->active_scene->game_objects) {
            if (ui_is_selected.find(go->getUUID()) == ui_is_selected.end()) {
                ui_is_selected.insert(std::make_pair(go->getUUID(), false));
            }
            if (ImGui::Selectable(go->getName().c_str(), &ui_is_selected.at(go->getUUID()))) {
                if (global_ui_context->selected_game_object_uuid != go->getUUID()) {
                    selectGameObject(go->getUUID());
                } else {
                    selectGameObject();
                }
            }
        }
        ImGui::End();
    }

    void HierarchyUINode::selectGameObject(MatchEngine::Game::GameObjectUUID uuid) {
        if (global_ui_context->selected_game_object_uuid != MatchEngine::Game::GameObjectUUID(-1)) {
            ui_is_selected.at(global_ui_context->selected_game_object_uuid) = false;
        }
        global_ui_context->selected_game_object_uuid = uuid;
        MatchEngine::global_runtime_context->render_system->getRenderer()->reportSelectedGameObject(global_ui_context->selected_game_object_uuid);
        if (uuid != MatchEngine::Game::GameObjectUUID(-1)) {
            ui_is_selected.at(uuid) = true;
        }
    }
}
