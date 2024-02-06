#include <MatchEngine/game_framework/game_object/game_object_resource.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine::Game {
    GameObjectResource::~GameObjectResource() {
        for (auto [uuid, component] : component_templates) {
            delete component;
        }
    }

    void GameObjectResource::load() {
        for (auto [uuid, component] : component_templates) {
            component->setupRTTI();
        }
    }
    void GameObjectResource::unload() {}

    void GameObjectResource::attachComponentTemplate(Component *component) {
        if (component_templates.find(component->getUUID()) != component_templates.end()) {
            MCH_CORE_ERROR("ComponentTemplate {} has been attached.", component->getUUID())
            return;
        }
        component_templates[component->getUUID()] = component;
    }

    void GameObjectResource::detachComponentTemplate(Component *component) {
        detachComponentTemplate(component->getUUID());
    }
    
    void GameObjectResource::detachComponentTemplate(ComponentTypeUUID uuid) {
        if (component_templates.find(uuid) == component_templates.end()) {
            MCH_CORE_ERROR("ComponentTemplate {} has been detached.", uuid)
            return;
        }
        component_templates.erase(uuid);
    }
 
    GameObject *GameObjectResource::createGameObject() {
        auto *game_object = new GameObject();
        for (auto [uuid, component] : component_templates) {
            game_object->getComponents().push_back(component->clone());
        }
        for (auto *component : game_object->getComponents()) {
            component->master = game_object;
            component->create();
        }
        return game_object;
    }
}