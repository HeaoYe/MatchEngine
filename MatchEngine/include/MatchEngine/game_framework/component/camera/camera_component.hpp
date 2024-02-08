#pragma once

#include <MatchEngine/game_framework/component/component.hpp>
#include <MatchEngine/function/render/render_target/render_target.hpp>
#include <glm/mat4x4.hpp>

namespace MatchEngine::Game {
    // 相机组件
    REFLECT_CLASS(NAME = "CameraComponent")
    class CameraComponent : public Component {
        DECLARE_COMPONENT(CameraComponent)
    private:
        void onStart() override;
        void onTick(float dt) override;
    public:
        glm::mat4 view;
        glm::mat4 project;
    protected:
        RenderTargetID render_target_id;
    };
}
