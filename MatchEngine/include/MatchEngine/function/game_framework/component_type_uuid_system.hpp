#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/game_framework/uuid_type.hpp>
#include <map>

namespace MatchEngine {
    // ComponentTypeUUID管理系统, 用于在运行时获取不同类型组件的UUID
    class ComponentTypeUUIDSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(ComponentTypeUUIDSystem)
    public:
        Game::ComponentTypeUUID get(const std::string &class_name) {
            if (auto iter = uuid_map.find(class_name); iter != uuid_map.end()) {
                return iter->second;
            }
            current_uuid ++;
            uuid_map.insert(std::make_pair(class_name, current_uuid));
            return current_uuid;
        }
    private:
        std::map<std::string, Game::ComponentTypeUUID> uuid_map;
        Game::ComponentTypeUUID current_uuid { 0 };
    };
}
