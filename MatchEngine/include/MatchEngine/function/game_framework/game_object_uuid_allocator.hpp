#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/game_framework/uuid_type.hpp>

namespace MatchEngine {
    // GameObjectUUID分配器, 为每个GameObject分配UUID
    class GameObjectUUIDAllocator final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(GameObjectUUIDAllocator)
    public:
        Game::GameObjectUUID allocate();
    private:
        Game::GameObjectUUID current_uuid;
    };
}
