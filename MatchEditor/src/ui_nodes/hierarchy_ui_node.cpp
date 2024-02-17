#include <ui_nodes/hierarchy_ui_node.hpp>

namespace MatchEditor {
    void HierarchyUINode::onLoadScene() {
        for (auto *go : MatchEngine::global_runtime_context->scene_manager->active_scene->game_objects) {
            global_ui_context->is_selected[go->getUUID()] = false;
        }
        global_ui_context->selected_game_object_uuid = MatchEngine::Game::GameObjectUUID(-1);
    }

    void HierarchyUINode::onUnloadScene() {
        global_ui_context->is_selected.clear();
    }

    void HierarchyUINode::render() {
        ImGui::Begin("Hierarchy");
        for (auto *go : MatchEngine::global_runtime_context->scene_manager->active_scene->game_objects) {
            if (global_ui_context->is_selected.find(go->getUUID()) == global_ui_context->is_selected.end()) {
                global_ui_context->is_selected[go->getUUID()] = false;
            }
            if (ImGui::Selectable(go->getName().c_str(), &global_ui_context->is_selected[go->getUUID()])) {
                global_ui_context->is_selected[global_ui_context->selected_game_object_uuid] = false;
                if (global_ui_context->is_selected[go->getUUID()]) {
                    global_ui_context->selected_game_object_uuid = go->getUUID();
                } else {
                    global_ui_context->selected_game_object_uuid = MatchEngine::Game::GameObjectUUID(-1);
                }
                MatchEngine::global_runtime_context->render_system->getRenderer()->reportSelectedGameObject(global_ui_context->selected_game_object_uuid);
            }
        }
        ImGui::End();
    }
}
