#include <MatchEngine/game_framework/component/camera/camera_component.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    CameraComponent::CameraComponent() {
        camera_id = global_runtime_context->camera_system->allocateCamera();
    }

    CameraComponent::~CameraComponent() {
        global_runtime_context->camera_system->removeCamera(camera_id);
        camera_id = 0;
    }
}
