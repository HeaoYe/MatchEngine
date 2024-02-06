#include <MatchEngine/function/game_framework/scene_manager.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine {
    SceneManager::SceneManager() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;
    }

    bool SceneManager::hasRegisteredScene(const std::string &name) {
        return scenes.find(name) != scenes.end();
    }

    void SceneManager::registerScene(std::unique_ptr<Game::Scene> scene) {
        scenes[scene->getSceneName()] = std::move(scene);
    }

    void SceneManager::loadScene(const std::string &name) {
        unloadScene();
        if (!hasRegisteredScene(name)) {
            MCH_CORE_ERROR("Scene {} hasn't been registered.", name)
            return;
        }
        active_scene = scenes.at(name).get();
        active_scene->load();
    }
    
    void SceneManager::unloadScene() {
        if (active_scene == nullptr) {
            return;
        }
        active_scene->unload();
        active_scene = nullptr;
    }

    void SceneManager::tick(float dt) {
        if (active_scene == nullptr) {
            return;
        }
        active_scene->tick(dt);
    }

    SceneManager::~SceneManager() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        scenes.clear();
    }
}
