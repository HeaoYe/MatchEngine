#pragma once

#include <MatchEngine/MatchEngine.hpp>
#include <glm/gtx/rotate_vector.hpp>

// 自定义组件

// 声明反射信息(必须声明, 且NAME = "类名")
REFLECT_CLASS(NAME = CameraController)
// 相机控制器组件
class CameraController : public MatchEngine::Game::Component {
    // 声明组件信息
    DECLARE_COMPONENT(CameraController)

    void onStart() override {
        camera = master->forceGetComponent<MatchEngine::Game::CameraComponent>();

        auto transform = master->queryComponent<MatchEngine::Game::TransformComponent>();
        if (transform == nullptr) {
            location = new glm::vec3();
        } else {
            location = &transform->location;
        }
        *location = { 2, 1, -4 };

        updateViewMatrix();
        camera->project = glm::perspective<float>(0.8, 1920 / 1080.0f, 0.1, 100);
        camera->uploadProjectMatrix();
        yaw = 0;
        pitch = 0;
    }

    void onTick(float dt) override {
        // 控制相机的移动
        using namespace MatchEngine::UserInterface;
        using MatchEngine::Key;
        using MatchEngine::MouseButton;
        auto delta = input->getMouseDelta() * 0.2;
        if (input->isMouseButtonPressed(MouseButton::eLeft)) {
            yaw -= delta.x;
            pitch = std::clamp<float>(pitch + delta.y, -89.0f, 89.0f);
            updateViewMatrix();
        }
        auto d = glm::rotateY(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(yaw));
        auto cd = glm::rotateY(d, glm::radians(90.0f));

        if (input->isKeyPressed(Key::eW)) {
            *location += d * dt;
            updateViewMatrix();
        }
        if (input->isKeyPressed(Key::eS)) {
            *location -= d * dt;
            updateViewMatrix();
        }
        if (input->isKeyPressed(Key::eA)) {
            *location += cd * dt;
            updateViewMatrix();
        }
        if (input->isKeyPressed(Key::eD)) {
            *location -= cd * dt;
            updateViewMatrix();
        }
        if (input->isKeyPressed(Key::eSpace)) {
            location->y += dt;
            updateViewMatrix();
        }
        if (input->isKeyPressed(Key::eLeftShift)) {
            location->y -= dt;
            updateViewMatrix();
        }
    }
private:
    void updateViewMatrix() {
        auto view_direction = glm::rotateY(glm::rotateX(glm::vec3(0, 0, 1), glm::radians(pitch)), glm::radians(yaw));
        camera->view = glm::lookAt(*location, *location + view_direction, { 0, 1, 0 });
        camera->uploadViewMatrix();
    }
private:
    MatchEngine::Game::CameraComponent *camera;
    glm::vec3 *location;
    float yaw, pitch;
};
