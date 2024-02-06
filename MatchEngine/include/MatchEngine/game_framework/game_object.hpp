#pragma once

#include <MatchEngine/game_framework/component/component.hpp>
#include <list>
#include <map>

namespace MatchEngine::Game {
    // GameObject 游戏对象, GameObject管理它的多种Component
    class GameObject final {
        NoCopyMoveConstruction(GameObject)
    public:
        GameObject(const std::string &name);
        ~GameObject();

        void start();
        void fixedTick();
        void tick(float dt);
        void postTick(float dt);

        void addComponet(Component *component);
        void removeComponent(Component *component);
        void removeComponent(ComponentTypeUUID uuid);

        Component *queryComponent(const std::string &class_name);
        Component *forceGetComponent(const std::string &class_name);
    public:
        GameObjectUUID getUUID() const { return uuid; }

        std::string getName() const { return name; }

        template <class C>
        C *forceGetComponent() {
            return static_cast<C *>(forceGetComponent(C::GetClassName()));
        }

        template <class C>
        C *queryComponent() {
            return static_cast<C *>(queryComponent(C::GetClassName()));
        }

        std::list<Component *> getComponets() { return components; }
    private:
        GameObjectUUID uuid;
        std::string name;
        std::map<ComponentTypeUUID, Component *> component_map;
        std::list<Component *> components;
    };
}
