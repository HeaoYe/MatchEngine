#pragma once

#include <MatchEngine/game_framework/uuid_type.hpp>
#include <MatchEngine/function/camera/camera_id.hpp>
#include <functional>

namespace MatchEditor {
    enum class Mode {
        eEdit,
        eGame,
    };

    // 编辑器UI的共享全局变量
    struct UIContext {
        Mode mode { Mode::eEdit };
        std::function<void(Mode mode)> change_mode_callback;

        MatchEngine::Game::GameObjectUUID selected_game_object_uuid;
        MatchEngine::CameraID viewport_camera_id;
    };

    extern UIContext *global_ui_context;
}
