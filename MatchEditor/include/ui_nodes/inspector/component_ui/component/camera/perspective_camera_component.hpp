#include <ui_nodes/inspector/component_ui/component_ui.hpp>
#include <MatchEngine/game_framework/component/camera/perspective_camera_component.hpp>

namespace MatchEditor {
    // PerspectiveCameraComponent的数据视图, 用于连接Component和对应的UI, 任何读取和写入都需要通过View
    template <>
    class ComponentView<MatchEngine::Game::PerspectiveCameraComponent> {
    public:
        ComponentView(MatchEngine::Game::PerspectiveCameraComponent &camera) : camera(camera) {
            primary = false;
        }

        MatchEngine::CameraID getCameraID() const { return camera.camera_id; }
    public:
        bool primary;
    private:
        MatchEngine::Game::PerspectiveCameraComponent &camera;
    };

    // 透视相机的ComponentUI
    template <>
    class ComponentUI<MatchEngine::Game::PerspectiveCameraComponent> {
    public:
        void render(ComponentView<MatchEngine::Game::PerspectiveCameraComponent> &view) {
            ImGui::PushID(size_t(&view));
            if (ImGui::TreeNodeEx("透视相机")) {

                if (ImGui::Checkbox("主视角相机", &view.primary)) {
                    if (view.primary) {
                        MatchEngine::global_runtime_context->camera_system->reportCameraAsPrimaryViewport(view.getCameraID());
                    }
                }

                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    };
}
