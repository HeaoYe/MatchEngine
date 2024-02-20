#include <MatchEngine/game_framework/scene.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine::Game {
    Scene::~Scene() {
        for (auto *game_object : game_objects) {
            delete game_object;
        }
        game_objects.clear();
        game_object_map.clear();
    }

    void Scene::awake() {
        for (auto *game_object : game_objects) {
            game_object->awake();
        }
    }

    void Scene::start() {
        for (auto *game_object : game_objects) {
            game_object->start();
        }
    }

    void Scene::fixedTick() {
        for (auto *game_object : game_objects) {
            game_object->fixedTick();
        }
    }
    
    void Scene::tick(float dt) {
        for (auto *game_object : game_objects) {
            game_object->tick(dt);
        }
    }
    
    void Scene::postTick(float dt) {
        for (auto *game_object : game_objects) {
            game_object->postTick(dt);
        }
    }

    PointerWrapper<GameObject> Scene::createGameObject(const std::string &name) {
        auto *game_object = new GameObject(name);
        game_object_map.insert(std::make_pair(game_object->getUUID(), game_object));
        game_objects.push_back(game_object);
        return game_object;
    }

    void Scene::removeGameObject(GameObject *game_object) {
        removeGameObject(game_object->getUUID());
    }

    void Scene::removeGameObject(GameObjectUUID uuid) {
        if (auto iter = game_object_map.find(uuid); iter != game_object_map.end()) {
            game_objects.remove(iter->second);
            game_object_map.erase(iter);
            return;
        }
        MCH_CORE_ERROR("Game Object {} has been removed.", uuid)
    }
}
