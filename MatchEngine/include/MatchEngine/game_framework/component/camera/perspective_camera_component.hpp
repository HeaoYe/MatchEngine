#pragma once

#include <MatchEngine/game_framework/component/camera/camera_component.hpp>

namespace MatchEngine::Game {
    // 透视投行相机，控制Project矩阵
    REFLECT_CLASS(NAME = "PerspectiveCameraComponent")
    class PerspectiveCameraComponent : public CameraComponent {
        DECLARE_COMPONENT_CUSTOM_CONSTRUCTION(PerspectiveCameraComponent)
    public:
        PerspectiveCameraComponent(float fovDegrees, float aspect, float near, float far);
        PerspectiveCameraComponent(float fovDegrees, float width, float height, float near, float far);

        void onStart() override;
        
        void calculatePerspectiveMatrix(float fovDegrees, float aspect, float near, float far);
        void calculatePerspectiveMatrix(float fovDegrees, float width, float height, float near, float far);
    };
}
