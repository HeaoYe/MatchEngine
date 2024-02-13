#include <ui_nodes/hierarchy_ui_node.hpp>

namespace MatchEditor {
    void HierarchyUINode::onLoadScene() {
        for (auto *go : MatchEngine::global_runtime_context->scene_manager->active_scene->game_objects) {
            is_selected[go->getUUID()] = false;
        }
        global_ui_context->selected_game_object_uuid = MatchEngine::Game::GameObjectUUID(-1);
    }

    void HierarchyUINode::onUnloadScene() {
        is_selected.clear();
    }

    void HierarchyUINode::render() {
        ImGui::Begin("Hierarchy");
        for (auto *go : MatchEngine::global_runtime_context->scene_manager->active_scene->game_objects) {
            if (is_selected.find(go->getUUID()) == is_selected.end()) {
                is_selected[go->getUUID()] = false;
            }
            if (ImGui::Selectable(go->getName().c_str(), &is_selected[go->getUUID()])) {
                is_selected[global_ui_context->selected_game_object_uuid] = false;
                if (is_selected[go->getUUID()]) {
                    global_ui_context->selected_game_object_uuid = go->getUUID();
                } else {
                    global_ui_context->selected_game_object_uuid = MatchEngine::Game::GameObjectUUID(-1);
                }
            }
        }
        ImGui::End();
    }
}
