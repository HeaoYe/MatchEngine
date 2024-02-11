#include <MatchEngine/function/game_framework/component_type_uuid_system.hpp>

namespace MatchEngine {
    ComponentTypeUUIDSystem::ComponentTypeUUIDSystem() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;
    }

    ComponentTypeUUIDSystem::~ComponentTypeUUIDSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;
    }
}
