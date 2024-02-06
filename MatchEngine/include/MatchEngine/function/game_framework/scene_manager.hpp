#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/game_framework/scene.hpp>
#include <memory>

namespace MatchEngine {
    // 场景管理器
    class SceneManager final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(SceneManager)
    public:
        bool hasRegisteredScene(const std::string &name);
        void registerScene(std::unique_ptr<Game::Scene> scene);
        void loadScene(const std::string &name);
        void unloadScene();

        void tick(float dt);
    private:
        std::map<std::string, std::unique_ptr<Game::Scene>> scenes;
        Game::Scene *active_scene { nullptr };
    };
}
