#include <editor.hpp>
#include <MatchEngine/../../src/MatchEngine/internal.hpp>

namespace MatchEditor {
    MatchEditor::MatchEditor(MatchEngine::MatchEngine *engine) : engine(engine) {
        engine->editor_mode = true;
        engine->hook_imgui_layer = [this]() {
            ui = std::make_unique<UI>();
            global_ui_context->change_mode_callback = [this](Mode mode) {
                switch (mode) {
                case Mode::eEdit:
                    global_ui_context->mode = Mode::eEdit;
                    MatchEngine::global_runtime_context->camera_system->activeEditorCamera(global_ui_context->viewport_camera_id);
                    this->engine->stop();
                    break;
                case Mode::eGame:
                    global_ui_context->mode = Mode::eGame;
                    this->engine->start();
                    MatchEngine::global_runtime_context->camera_system->deactiveEditorCamera();
                    break;
                }
            };
        };
    }

    void MatchEditor::initialize() {
        if (MatchEngine::global_runtime_context == nullptr) {
            engine->initialize();
        }
        if (MatchEngine::global_runtime_context->scene_manager->scenes.empty()) {
            MatchEngine::global_runtime_context->scene_manager->createScene("MatchEditor Default Scene");
        }
        ui->onLoadScene();
    }

    void MatchEditor::destroy() {
        ImGui::SaveIniSettingsToDisk("assets/ui/imgui.ini");
        ui.reset();
    }

    void MatchEditor::gameLoop() {
        auto ui_timer = MatchEngine::global_runtime_context->timer_system->createTimer();
        while (MatchEngine::global_runtime_context->window_system->isAlive()) {
            MatchEngine::global_runtime_context->window_system->pollEvents();
            // 游戏运行分为编辑模式和游戏模式
            switch (global_ui_context->mode) {
            case Mode::eEdit:
                ui->onTick(ui_timer->tick());
                break;
            case Mode::eGame:
                engine->tickLogic();
                break;
            }
            engine->tickRender();
            engine->tickInputState();
        }
        engine->waitFixedTickThread();
    }

    MatchEditor::~MatchEditor() {
        engine = nullptr;
    }
}
