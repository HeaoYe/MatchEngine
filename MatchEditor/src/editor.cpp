#include <editor.hpp>
#include <MatchEngine/../../src/MatchEngine/internal.hpp>

namespace MatchEditor {
    MatchEditor::MatchEditor(MatchEngine::MatchEngine *engine) {
        MatchEngine::Renderer::output_attachment_name = "RenderOutput";
    }

    void MatchEditor::initialize() {
        if (MatchEngine::global_runtime_context == nullptr) {
            engine->initialize();
        }
        if (MatchEngine::global_runtime_context->scene_manager->scenes.empty()) {
            MatchEngine::global_runtime_context->scene_manager->createScene("MatchEditor Default Scene");
        }
        ui = std::make_unique<UI>();
        ui->onLoadScene();
    }

    void MatchEditor::destroy() {
        ImGui::SaveIniSettingsToDisk("assets/ui/imgui.ini");
        ui.reset();
    }

    MatchEditor::~MatchEditor() {}
}
