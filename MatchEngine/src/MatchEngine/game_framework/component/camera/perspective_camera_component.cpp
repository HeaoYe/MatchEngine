#include <MatchEngine/game_framework/component/camera/perspective_camera_component.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace MatchEngine::Game {
    PerspectiveCameraComponent::PerspectiveCameraComponent(float fovDegrees, float aspect, float near, float far) {
        calculatePerspectiveMatrix(fovDegrees, aspect, near, far);
    }

    PerspectiveCameraComponent::PerspectiveCameraComponent(float fovDegrees, float width, float height, float near, float far) {
        calculatePerspectiveMatrix(fovDegrees, width, height, near, far);
    }

    void PerspectiveCameraComponent::calculatePerspectiveMatrix(float fovDegrees, float aspect, float near, float far) {
        project = glm::perspective(glm::radians(fovDegrees), aspect, near, far);
        uploadProjectMatrix();
    }
    
    void PerspectiveCameraComponent::calculatePerspectiveMatrix(float fovDegrees, float width, float height, float near, float far) {
        project = glm::perspectiveFov(glm::radians(fovDegrees), width, height, near, far);
        uploadProjectMatrix();
    }
}
