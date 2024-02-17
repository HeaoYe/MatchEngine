#pragma once

#include <MatchEngine/game_framework/uuid_type.hpp>
#include <map>

namespace MatchEditor {
    // 编辑器UI的共享全局变量
    struct UIContext {
        MatchEngine::Game::GameObjectUUID selected_game_object_uuid;
        std::map<MatchEngine::Game::GameObjectUUID, bool> is_selected;
    };

    extern UIContext *global_ui_context;
}
