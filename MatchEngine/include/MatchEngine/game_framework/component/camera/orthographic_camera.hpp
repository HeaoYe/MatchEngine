#pragma once

#include <MatchEngine/game_framework/component/camera/camera_component.hpp>

namespace MatchEngine::Game {
    // 正交投行相机，控制Project矩阵
    REFLECT_CLASS(NAME = "OrthographicCameraComponent")
    class OrthographicCameraComponent : public CameraComponent {
        DECLARE_COMPONENT_CUSTOM_CONSTRUCTION(OrthographicCameraComponent)
    public:
        OrthographicCameraComponent(float left, float right, float bottom, float top, float near, float far);

        void onStart() override;

        void calculateOrthographicMatrix(float left, float right, float bottom, float top, float near, float far);
    };
}
