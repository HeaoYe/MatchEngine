#include <MatchEngine/function/game_framework/game_object_uuid_allocator.hpp>

namespace MatchEngine {
    GameObjectUUIDAllocator::GameObjectUUIDAllocator() {
        initializeRuntimeSystem();

        current_uuid = 0;

        state = RuntimeSystem::State::eInitialized;
    }
    
    Game::GameObjectUUID GameObjectUUIDAllocator::allocate() {
        current_uuid ++;
        return current_uuid;
    }
    
    GameObjectUUIDAllocator::~GameObjectUUIDAllocator() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;
    }
}
