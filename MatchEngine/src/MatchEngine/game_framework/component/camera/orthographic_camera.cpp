#include <MatchEngine/game_framework/component/camera/orthographic_camera.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace MatchEngine::Game {
    OrthographicCameraComponent::OrthographicCameraComponent(float left, float right, float bottom, float top, float near, float far) {
        calculateOrthographicMatrix(left, right, bottom, top, near, far);
    }

    void OrthographicCameraComponent::calculateOrthographicMatrix(float left, float right, float bottom, float top, float near, float far) {
        project = glm::ortho(left, right, bottom, top, near, far);
        uploadProjectMatrix();
    }
}
