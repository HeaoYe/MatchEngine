#pragma once

#include <MatchEngine/game_framework/game_object/game_object.hpp>
#include <string>
#include <map>

namespace MatchEngine::Game {
    // GameObject资源, 用于定义一种GameObject, 可以定义一种GameObject身上的组件有哪些, 以及组件创建时的初始状态
    class GameObjectResource final {
        NoCopyMoveConstruction(GameObjectResource)
    public:
        GameObjectResource(const std::string &name) : name(name) {}
        ~GameObjectResource();

        void load();
        void unload();

        void attachComponentTemplate(Component *component);
        void detachComponentTemplate(Component *component);
        void detachComponentTemplate(ComponentTypeUUID uuid);

        GameObject *createGameObject();

        std::string getResourceName() const { return name; }
    private:
        std::string name;
        std::map<ComponentTypeUUID, Component *> component_templates;
    };
}
