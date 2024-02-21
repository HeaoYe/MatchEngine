#include <MatchEngine/function/user_interface/user_interface.hpp>

namespace MatchEngine::UserInterface {
    PointerWrapper<ReflectSystem> reflect { nullptr };
    PointerWrapper<LoggerSystem> logger_system { nullptr };
    PointerWrapper<EventSystem> event_system { nullptr };
    PointerWrapper<InputSystem> input {nullptr};
    PointerWrapper<TimerSystem> timer_system { nullptr };
    PointerWrapper<CameraSystem> camera_system { nullptr };
    PointerWrapper<SceneManager> scene_manager { nullptr };
    PointerWrapper<AssetSystem> assets_system { nullptr };
}
