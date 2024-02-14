#include <ui_nodes/setting_ui_node.hpp>
#include <MatchEngine/function/render/pass/mesh_pass.hpp>

namespace MatchEditor {
    void SettingUINode::render() {
        ImGui::Begin("Setting");

        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        auto mesh_pass = static_cast<MatchEngine::MeshPass *>(MatchEngine::global_runtime_context->render_system->getActiveSceneRenderer()->subpasses.front().get());
        ImGui::Text("Visibility: %d", mesh_pass->counts_ptrs[Match::runtime_setting->current_in_flight][3]);
        
        ImGui::End();
    }
}
