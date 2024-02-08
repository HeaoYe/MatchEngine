#include <MatchEngine/game_framework/component/camera/camera_component.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    void CameraComponent::onStart() {
        render_target_id = global_runtime_context->render_system->getActiveSceneRenderer()->getRenderTargetPool()->createRenderTarget(RenderTargetFormat::eRGBA32float);
    }

    void CameraComponent::onTick(float dt) {
        global_runtime_context->render_system->getActiveSceneRenderer()->addDrawCall(this, render_target_id);
    }
}
