#include <MatchEngine/game_framework/game_object/game_object_resource_container.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine::Game {
    GameObjectResourceContainer::~GameObjectResourceContainer() {
        unload();
    }

    void GameObjectResourceContainer::attachGameObjectResource(PointerWrapper<GameObjectResource> resource) {
        if (game_object_resources.find(resource->getResourceName()) != game_object_resources.end()) {
            MCH_CORE_ERROR("GameObjectResource {} has been attached.", resource->getResourceName())
            return;
        }
        game_object_resources.insert(std::make_pair(resource->getResourceName(), resource));
    }

    void GameObjectResourceContainer::detachGameObjectResource(const std::string &name) {
        if (game_object_resources.find(name) == game_object_resources.end()) {
            MCH_CORE_ERROR("GameObjectResource {} has been detached.", name)
            return;
        }
        game_object_resources.erase(name);
    }

    bool GameObjectResourceContainer::hasGameObjectResource(const std::string &resource_name) {
        return game_object_resources.find(resource_name) != game_object_resources.end();
    }
    
    GameObject *GameObjectResourceContainer::createGameObject(const std::string &resource_name) {
        if (!hasGameObjectResource(resource_name)) {
            return nullptr;
        }
        return forceCreateGameObject(resource_name);
    }

    GameObject *GameObjectResourceContainer::forceCreateGameObject(const std::string &resource_name) {
        return game_object_resources.at(resource_name)->createGameObject();
    }

    void GameObjectResourceContainer::load() {
        if (isLoaded) {
            return;
        }
        for (auto &[name, resource] : game_object_resources) {
            resource->load();
        }
        isLoaded = true;
    }

    void GameObjectResourceContainer::unload() {
        if (!isLoaded) {
            return;
        }
        for (auto &[name, resource] : game_object_resources) {
            resource->unload();
        }
        isLoaded = false;
    }
}
