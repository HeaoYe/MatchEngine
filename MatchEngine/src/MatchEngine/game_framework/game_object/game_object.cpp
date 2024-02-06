#include <MatchEngine/game_framework/game_object/game_object.hpp>

namespace MatchEngine::Game {
    GameObject::~GameObject() {
        for (auto *child : children) {
            delete child;
        }
        children.clear();
        for (auto *component : components) {
            component->destroy();
            delete component;
        }
        components.clear();
    }

    GameObject *GameObject::addChild(GameObject *game_object) {
        game_object->master = this;
        children.push_back(game_object);
        return game_object;
    }

    GameObject *GameObject::removeChild(GameObject *game_object) {
        children.remove(game_object);
        game_object->master = nullptr;
        return game_object;
    }

    void GameObject::tick(float dt) {
        for (auto *component : components) {
            component->onTick(dt);
        }
        for (auto *child : children) {
            child->tick(dt);
        }
    }
 
    Component *GameObject::findComponent(ComponentTypeUUID uuid) {
        for (auto *component : components) {
            if (component->uuid == uuid) {
                return component;
            }
        }
        return nullptr;
    }
}
