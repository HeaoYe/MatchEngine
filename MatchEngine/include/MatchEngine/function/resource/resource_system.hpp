#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/game_framework/scene.hpp>
#include <MatchEngine/game_framework/level.hpp>

namespace MatchEngine {
    // 资源系统,管理游戏中的资源
    class ResourceSystem : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(ResourceSystem)
    public:
        PointerWrapper<Game::Scene> createScene(const std::string &name);
        PointerWrapper<Game::GameObjectResource> createGameObjectResource(const std::string &name);
    private:
        std::vector<Game::GameObjectResource *> game_object_resources;
    };
}
