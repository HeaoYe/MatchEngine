#pragma once

#include <MatchEngine/game_framework/component/component.hpp>
#include <MatchEngine/function/game_framework/component_type_uuid_system.hpp>
#include <vector>
#include <list>

namespace MatchEngine::Game {
    // GameObject 游戏对象, GameObject可以添加多个GameObject作为子对象, GameObject还管理它的多种Component
    class GameObject final {
        DefaultNoCopyMoveConstruction(GameObject)
    public:
        ~GameObject();

        GameObject *addChild(GameObject *game_object);
        GameObject *removeChild(GameObject *game_object);

        void tick(float dt);
    public:
        template <class C>
        C *findComponent() {
            return static_cast<C *>(findComponent(UserInterface::component_type_uuid_system->getUUID<C>()));
        }
        Component *findComponent(ComponentTypeUUID uuid);
        std::vector<Component *> &getComponents() { return components; }
        const std::vector<Component *> &getComponents() const { return components; }
    private:
        GameObject *master;
        std::vector<Component *> components;
        std::list<GameObject *> children;
    };
}
