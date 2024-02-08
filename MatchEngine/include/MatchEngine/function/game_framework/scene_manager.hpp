#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/game_framework/scene.hpp>


#include <MatchEngine/function/render/mesh/mesh.hpp>


namespace MatchEngine {
    // 场景管理器, 管理多个场景
    class SceneManager final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(SceneManager)
    public:
        bool hasRegisteredScene(const std::string &name);
        PointerWrapper<Game::Scene> createScene(const std::string &name);
        void loadScene(const std::string &name);

        // Test Function
        MeshID loadMesh(const std::string &filename);

        void start();
        void fixedTick();
        void tick(float dt);
        void swap();

        std::string getActiveSceneName() const  { return active_scene->getSceneName(); }
    private:
        void setActiveScene(Game::Scene *scene);
    private:
        std::map<std::string, Game::Scene *> scenes;
        Game::Scene *active_scene { nullptr };
        Game::Scene *change_scene { nullptr };
    };
}
