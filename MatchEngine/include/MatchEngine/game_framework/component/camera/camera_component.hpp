#pragma once

#include <MatchEngine/game_framework/component/component.hpp>
#include <MatchEngine/function/camera/camera_id.hpp>

namespace MatchEngine::Game {
    // 相机组件
    REFLECT_CLASS(NAME = "CameraComponent")
    class CameraComponent : public Component {
        DECLARE_COMPONENT_CUSTOM_CONSTRUCTION(CameraComponent)
    public:
        CameraComponent();
        ~CameraComponent() override;
    public:
        CameraID camera_id;
    };
}
