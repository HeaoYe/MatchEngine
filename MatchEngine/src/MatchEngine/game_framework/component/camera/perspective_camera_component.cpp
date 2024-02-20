#include <MatchEngine/game_framework/component/camera/perspective_camera_component.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    PerspectiveCameraComponent::PerspectiveCameraComponent(float fovDegrees, float aspect, float near, float far) : CameraComponent() {
        calculatePerspectiveMatrix(fovDegrees, aspect, near, far);
    }

    PerspectiveCameraComponent::PerspectiveCameraComponent(float fovDegrees, float width, float height, float near, float far) : CameraComponent() {
        calculatePerspectiveMatrix(fovDegrees, width, height, near, far);
    }

    void PerspectiveCameraComponent::calculatePerspectiveMatrix(float fovDegrees, float aspect, float near, float far) {
        global_runtime_context->camera_system->reportCameraProjectMatrix(camera_id, glm::perspective(glm::radians(fovDegrees), aspect, near, far), near, far);
    }
    
    void PerspectiveCameraComponent::calculatePerspectiveMatrix(float fovDegrees, float width, float height, float near, float far) {
        global_runtime_context->camera_system->reportCameraProjectMatrix(camera_id, glm::perspectiveFov(glm::radians(fovDegrees), width, height, near, far), near, far);
    }
}
