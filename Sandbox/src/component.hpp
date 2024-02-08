#pragma once

#include <MatchEngine/MatchEngine.hpp>

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
    }

    void onTick(float dt) override {
        // 控制相机的移动
        using namespace MatchEngine::UserInterface;
        using MatchEngine::Key;
        if (input->isKeyPressed(Key::eW)) {
            location->z += dt;
            updateViewMatrix();
        }
        if (input->isKeyPressed(Key::eS)) {
            location->z -= dt;
            updateViewMatrix();
        }
        if (input->isKeyPressed(Key::eD)) {
            location->x -= dt;
            updateViewMatrix();
        }
        if (input->isKeyPressed(Key::eA)) {
            location->x += dt;
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
        glm::vec3 view_direction = { 0, 0, 1 };
        camera->view = glm::lookAt(*location, { 0, 0, 0 }, { 0, 1, 0 });
    }
private:
    MatchEngine::Game::CameraComponent *camera;
    glm::vec3 *location;
};
