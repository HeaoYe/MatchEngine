#include <MatchEngine/function/game_framework/scene_manager.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include "internal.hpp"

namespace MatchEngine {
    SceneManager::SceneManager() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;
    }

    bool SceneManager::hasRegisteredScene(const std::string &name) {
        return scenes.find(name) != scenes.end();
    }

    PointerWrapper<Game::Scene> SceneManager::createScene(const std::string &name) {
        if (scenes.find(name) != scenes.end()) {
            MCH_CORE_ERROR("Scene {} has been created.", name)
            return nullptr;
        }
        auto *scene = new Game::Scene(name);
        scenes.insert(std::make_pair(name, scene));
        if (active_scene == nullptr) {
            setActiveScene(scene);
        }
        return scene;
    }

    void SceneManager::loadScene(const std::string &name) {
        if (auto iter = scenes.find(name); iter != scenes.end()) {
            change_scene = iter->second;
            return;
        }
        MCH_CORE_ERROR("No scene named {}.", name);
    }
    
    void SceneManager::start() {
        active_scene->start();
        global_runtime_context->render_system->postActiveSceneStart();
    }
    
    void SceneManager::fixedTick() {
        active_scene->fixedTick();
    }
    
    void SceneManager::tick(float dt) {
        active_scene->tick(dt);
        active_scene->postTick(dt);
    }
    
    void SceneManager::swap() {
        if (change_scene == nullptr) {
            return;
        }
        setActiveScene(change_scene);
        change_scene = nullptr;
        start();
    }
    
    void SceneManager::setActiveScene(Game::Scene *scene) {
        if (active_scene != nullptr) {
            global_runtime_context->render_system->destoryActiveSceneRenderer();
        }
        active_scene = scene;
        global_runtime_context->render_system->createActiveSceneRenderer();
    }
    
    SceneManager::~SceneManager() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        for (auto [name, scene] : scenes) {
            delete scene;
        }
        scenes.clear();
    }
}
