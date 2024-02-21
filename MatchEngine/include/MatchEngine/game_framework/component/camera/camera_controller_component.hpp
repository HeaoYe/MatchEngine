#pragma once

#include <MatchEngine/game_framework/component/camera/camera_component.hpp>
#include <MatchEngine/game_framework/component/transform/transform_component.hpp>

namespace MatchEngine::Game {
    // 相机控制器，控制相机的位置，视角（View矩阵），需要同时添加相机组件才可以工作
    REFLECT_CLASS(NAME = "CameraControllerComponent")
    class CameraControllerComponent : public Component {
        DECLARE_COMPONENT(CameraControllerComponent)
    public:
        void onStart() override;
        void onTick(float dt) override;
        ~CameraControllerComponent() override;
    private:
        void updateViewMatrix();
    public:
        CameraComponent *camera_component;
        TransformComponent *transform_component;
        glm::vec3 *location;
        float *yaw, *pitch;

        REFLECT_MEMBER()
        float speed { 10 };

        REFLECT_MEMBER()
        float sensitivity { 10 };
    };
}
