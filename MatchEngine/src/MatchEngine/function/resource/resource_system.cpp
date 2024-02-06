#include <MatchEngine/function/resource/resource_system.hpp>
#include "internal.hpp"

namespace MatchEngine {
    ResourceSystem::ResourceSystem() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;
    }

    PointerWrapper<Game::Scene> ResourceSystem::createScene(const std::string &name) {
        if (global_runtime_context->scene_manager->hasRegisteredScene(name)) {
            MCH_CORE_ERROR("Scene {} has been created.", name)
            return nullptr;
        }
        auto scene = std::make_unique<Game::Scene>(name);
        auto ptr = scene.get();
        global_runtime_context->scene_manager->registerScene(std::move(scene));
        return ptr;
    }

    PointerWrapper<Game::GameObjectResource> ResourceSystem::createGameObjectResource(const std::string &name) {
        game_object_resources.push_back(new Game::GameObjectResource(name));
        return game_object_resources.back();
    }

    ResourceSystem::~ResourceSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        for (auto *game_object_resource : game_object_resources) {
            game_object_resource->unload();
            delete game_object_resource;
        }
        game_object_resources.clear();
    }
}