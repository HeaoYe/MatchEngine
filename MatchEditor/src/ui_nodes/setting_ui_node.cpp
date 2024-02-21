#include <ui_nodes/setting_ui_node.hpp>

namespace MatchEditor {
    void SettingUINode::render() {
        ImGui::Begin("Setting");

        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        ImGui::Text("Visibility: %d", MatchEngine::global_runtime_context->render_system->getRenderer()->resource->visibility_count);
        ImGui::Text("DrawCall: %d", MatchEngine::global_runtime_context->render_system->getRenderer()->resource->draw_call_count);

        if (ImGui::Checkbox("垂直同步", &Match::runtime_setting->vsync)) {
            MatchEngine::global_runtime_context->render_system->getRenderer()->renderer->set_resize_flag();
        }

        switch (global_ui_context->mode) {
        case Mode::eEdit:
            if (ImGui::Button("开始游戏")) {
                global_ui_context->change_mode_callback(Mode::eGame);
            }
            break;
        case Mode::eGame:
            if (ImGui::Button("结束游戏")) {
                global_ui_context->change_mode_callback(Mode::eEdit);
            }
            break;
        }
        
        ImGui::End();
    }
}
