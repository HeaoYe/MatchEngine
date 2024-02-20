#include <MatchEngine/game_framework/game_object.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    GameObject::GameObject(const std::string &name) : name(name) {
        uuid = global_runtime_context->game_object_uuid_allocator->allocate();
    }

    GameObject::~GameObject() {
        for (auto *component : components) {
            component->onDestroy();
            delete component;
        }
        components.clear();
        component_map.clear();
    }

    void GameObject::awake() {
        for (auto *component : components) {
            component->onAwake();
        }
    }

    void GameObject::start() {
        for (auto *component : components) {
            component->onStart();
        }
    }

    void GameObject::fixedTick() {
        for (auto *component : components) {
            component->onFixedTick();
        }
    }

    void GameObject::tick(float dt) {
        for (auto *component : components) {
            component->onTick(dt);
        }
    }

    void GameObject::postTick(float dt) {
        for (auto *component : components) {
            component->onPostTick(dt);
        }
    }

    void GameObject::addComponet(Component *component) {
        component->uuid = global_runtime_context->component_type_uuid_system->get(component->getClassName());
        if (component_map.find(component->getTypeUUID()) != component_map.end()) {
            MCH_CORE_ERROR("Component {} has been added.", component->getTypeUUID())
            return;
        }
        component->master = this;
        component->setupRTTI();
        component_map.insert(std::make_pair(component->getTypeUUID(), component));
        components.push_back(component);
        component->onCreate();
    }

    void GameObject::removeComponent(Component *component) {
        removeComponent(component->getTypeUUID());
    }
    
    void GameObject::removeComponent(ComponentTypeUUID uuid) {
        if (auto iter = component_map.find(uuid); iter != component_map.end()) {
            components.remove(iter->second);
            component_map.erase(iter);
            return;
        }
        MCH_CORE_ERROR("Component {} has been removed.", uuid)
    }

    Component *GameObject::queryComponent(const std::string &class_name) {
        auto uuid = global_runtime_context->component_type_uuid_system->get(class_name);
        if (auto iter = component_map.find(uuid); iter != component_map.end()) {
            return component_map.at(uuid);
        }
        return nullptr;
    }

    Component *GameObject::forceGetComponent(const std::string &class_name) {
        return component_map.at(global_runtime_context->component_type_uuid_system->get(class_name));
    }
}
