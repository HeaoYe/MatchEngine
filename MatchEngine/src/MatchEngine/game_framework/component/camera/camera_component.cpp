#include <MatchEngine/game_framework/component/camera/camera_component.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    void CameraComponent::uploadViewMatrix() {
        global_runtime_context->render_system->getSwapData()->uploadCameraViewMatrix(view);
    }

    void CameraComponent::uploadProjectMatrix() {
        global_runtime_context->render_system->getSwapData()->uploadCameraProjectMatrix(project);
    }

    void CameraComponent::uploadNearFar() {
        global_runtime_context->render_system->getSwapData()->uploadCameraNearFar(near, far);
    }
}
