#include <MatchEngine/game_framework/component/camera/camera_component.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    void CameraComponent::uploadViewMatrix() {
        global_runtime_context->render_system->getActiveSceneRenderer()->getSwapData()->uploadCameraViewMatrix(view);
    }

    void CameraComponent::uploadProjectMatrix() {
        global_runtime_context->render_system->getActiveSceneRenderer()->getSwapData()->uploadCameraProjectMatrix(project);
    }
}
