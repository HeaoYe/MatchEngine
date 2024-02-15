#pragma once

#include <MatchEngine/game_framework/uuid_type.hpp>

namespace MatchEditor {
    // 编辑器UI的共享全局变量
    struct UIContext {
        MatchEngine::Game::GameObjectUUID selected_game_object_uuid;
    };

    extern UIContext *global_ui_context;
}
