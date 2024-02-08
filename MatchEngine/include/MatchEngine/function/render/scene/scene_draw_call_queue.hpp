#pragma once

#include <MatchEngine/game_framework/component/camera/camera_component.hpp>
#include <MatchEngine/function/render/render_target/render_target.hpp>
#include <queue>

namespace MatchEngine {
    // DrawCall队列, 有可能有多个相机, 所以一帧需要多次DrawCall队列渲染每个相机的画面
    class SceneDrawCallQueue {
        NoCopyMoveConstruction(SceneDrawCallQueue)
        friend class SceneRenderer;
    private:
        struct DrawCallData {
            alignas(16) glm::mat4 view;
            alignas(16) glm::mat4 project;
            alignas(4) RenderTargetID render_target_id;
        };
    public:
        SceneDrawCallQueue(class SceneRenderer *master);
        void addDrawCallData(Game::CameraComponent *camera, RenderTargetID render_target_id);
        DrawCallData fetchDrawCallData();
        bool empty() const;
    private:
        class SceneRenderer *master;
        std::queue<DrawCallData> draw_call_queue;
    };
}
