#pragma once

#include <MatchEngine/game_framework/component/component_type_uuid.hpp>
#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>

namespace MatchEngine {
    // 组件唯一ID管理系统, 用于在运行时获取不同类型组件的UUID
    class ComponentTypeUUIDSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(ComponentTypeUUIDSystem)
    public:
        template <typename T>
        Game::ComponentTypeUUID getUUID() {
            static Game::ComponentTypeUUID uuid = [&]() {
                current_uuid += 1;
                return current_uuid;
            }();
            return uuid;
        }
    private:
        Game::ComponentTypeUUID current_uuid { 0 };
    };

    namespace UserInterface {
        extern PointerWrapper<ComponentTypeUUIDSystem> component_type_uuid_system;
    }
}
