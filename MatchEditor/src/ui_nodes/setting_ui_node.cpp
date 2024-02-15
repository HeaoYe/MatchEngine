#include <ui_nodes/setting_ui_node.hpp>

namespace MatchEditor {
    void SettingUINode::render() {
        ImGui::Begin("Setting");

        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        ImGui::Text("Visibility: %d", MatchEngine::global_runtime_context->render_system->getRenderer()->resource->counts_ptrs[Match::runtime_setting->current_in_flight][3]);
        
        ImGui::End();
    }
}
