#include <ui_nodes/viewport/viewport_camera.hpp>
#include <ui_nodes/ui_context.hpp>
#include <MatchEngine/function/user_interface/user_interface.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace MatchEditor {
    ViewportCamera::ViewportCamera() {
        global_ui_context->viewport_camera_id = MatchEngine::UserInterface::camera_system->allocateCamera(true);
    }

    ViewportCamera::~ViewportCamera() {
        MatchEngine::UserInterface::camera_system->removeCamera(global_ui_context->viewport_camera_id);
    }

    void ViewportCamera::reset() {
        location = glm::vec3(0);
        yaw = 0;
        pitch = 0;
        updateViewMatrix();
        MatchEngine::UserInterface::camera_system->reportCameraProjectMatrix(global_ui_context->viewport_camera_id, glm::perspectiveFov<float>(glm::radians(60.0f), 1920, 1080, 0.01, 100), 0.01, 100, true);
    }

    void ViewportCamera::onTick(float dt) {
        using MatchEngine::MouseButton;
        using MatchEngine::Key;

        auto mouse_delta = static_cast<double>(sensitivity * dt) *  MatchEngine::UserInterface::input->getMouseDelta();
        bool changed = false;
        if (MatchEngine::UserInterface::input->isMouseButtonPressed(MouseButton::eRight)) {
            yaw -= mouse_delta.x;
            pitch = std::clamp<float>(pitch + mouse_delta.y, -89.0f, 89.0f);
            changed = true;
        }
        auto delta = dt * speed;
        auto tangent = delta * glm::rotateY(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(yaw));
        auto bitangent = glm::rotateY(tangent, glm::radians(90.0f));

        if (MatchEngine::UserInterface::input->isKeyPressed(Key::eW)) {
            location += tangent;
            changed = true;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(Key::eS)) {
            location -= tangent;
            changed = true;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(Key::eA)) {
            location += bitangent;
            changed = true;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(Key::eD)) {
            location -= bitangent;
            changed = true;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(Key::eSpace)) {
            location.y += delta;
            changed = true;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(Key::eLeftShift)) {
            location.y -= delta;
            changed = true;
        }
        if (changed) {
            updateViewMatrix();
        }
    }

    void ViewportCamera::updateViewMatrix() {
        auto view_direction = glm::rotateY(glm::rotateX(glm::vec3(0, 0, 1), glm::radians(pitch)), glm::radians(yaw));
        MatchEngine::UserInterface::camera_system->reportCameraViewMatrix(global_ui_context->viewport_camera_id, glm::lookAt(location, location + view_direction, { 0, 1, 0 }), true);
        auto data = MatchEngine::UserInterface::camera_system->quertyCameraData(global_ui_context->viewport_camera_id);
        auto p1 = data->view * glm::vec4(1, 1, 1, 1);
        p1 = data->view * glm::vec4(2, 1, 2, 1);
        p1 = data->view * glm::vec4(2, 3, 99, 1);
        p1 = data->view * glm::vec4(2, 3, 0, 1);
        p1 = data->view * glm::vec4(1, 3, -1, 1);
    }
}
