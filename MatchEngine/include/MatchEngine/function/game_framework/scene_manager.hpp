#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/game_framework/scene.hpp>

namespace MatchEngine {
    // 场景管理器, 管理多个场景
    class SceneManager final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(SceneManager)
    public:
        bool hasRegisteredScene(const std::string &name);
        PointerWrapper<Game::Scene> createScene(const std::string &name);
        void loadScene(const std::string &name);

        void start();
        void fixedTick();
        void tick(float dt);
        void swap();
    MATCHEDITOR_VISIBLE:
        std::map<std::string, Game::Scene *> scenes;
        Game::Scene *active_scene { nullptr };
        Game::Scene *change_scene { nullptr };
    };
}
