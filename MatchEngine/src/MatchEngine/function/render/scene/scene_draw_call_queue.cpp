#include <MatchEngine/function/render/scene/scene_draw_call_queue.hpp>
#include <MatchEngine/function/render/scene/scene_renderer.hpp>

namespace MatchEngine {
    SceneDrawCallQueue::SceneDrawCallQueue(SceneRenderer *master) : master(master) {};

    void SceneDrawCallQueue::addDrawCallData(Game::CameraComponent *camera, RenderTargetID render_target_id) {
        draw_call_queue.push(DrawCallData { camera->view, camera->project, render_target_id });
    }
    
    SceneDrawCallQueue::DrawCallData SceneDrawCallQueue::fetchDrawCallData() {
        DrawCallData data = draw_call_queue.front();
        draw_call_queue.pop();
        return data;
    }
 
    bool SceneDrawCallQueue::empty() const {
        return draw_call_queue.empty();
    }
}
