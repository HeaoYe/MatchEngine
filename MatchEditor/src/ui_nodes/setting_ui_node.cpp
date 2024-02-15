#include <ui_nodes/setting_ui_node.hpp>

namespace MatchEditor {
    void SettingUINode::render() {
        ImGui::Begin("Setting");

        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        ImGui::Text("Visibility: %d", MatchEngine::global_runtime_context->render_system->getRenderer()->resource->visibility_count);
        ImGui::Text("DrawCall: %d", MatchEngine::global_runtime_context->render_system->getRenderer()->resource->draw_call_count);
        static bool fiexed;
        if (ImGui::Checkbox("固定裁剪相机", &fiexed)) {
           MatchEngine::global_runtime_context->render_system->getSwapData()->setCameraFixed(fiexed);
        }

        if (ImGui::Checkbox("垂直同步", &Match::runtime_setting->vsync)) {
            MatchEngine::global_runtime_context->render_system->getRenderer()->renderer->set_resize_flag();
        }
        
        ImGui::End();
    }
}
