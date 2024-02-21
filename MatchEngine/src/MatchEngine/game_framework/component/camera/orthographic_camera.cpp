#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <MatchEngine/game_framework/component/camera/orthographic_camera.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    OrthographicCameraComponent::OrthographicCameraComponent(float left, float right, float bottom, float top, float near, float far) : CameraComponent() {
        calculateOrthographicMatrix(left, right, bottom, top, near, far);
    }

    void OrthographicCameraComponent::calculateOrthographicMatrix(float left, float right, float bottom, float top, float near, float far) {
        global_runtime_context->camera_system->reportCameraProjectMatrix(camera_id, glm::ortho(left, right, bottom, top, near, far), near, far);
    }
}
