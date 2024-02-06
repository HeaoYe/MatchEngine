#pragma once

#include <MatchEngine/game_framework/game_object/game_object_resource_container.hpp>

namespace MatchEngine::Game {
    // Level, 管理GameObject树结构
    class Level final {
        NoCopyMoveConstruction(Level)
        friend class Scene;
    public:
        Level(const std::string &name) : name(name) {}
        ~Level();

        void load();
        void unload();

        void tick(float dt);

        GameObject *createGameObject(const std::string &resource_name);

        std::string getLevelName() const { return name; }
        GameObjectResourceContainer &getGameObjectResourceContainer() { return game_object_resource_container; }
        GameObject &getRootGameObject() { return root_game_object; }
    private:
        std::string name;
        class Scene *master;
        GameObjectResourceContainer game_object_resource_container;
        GameObject root_game_object;
    };
}
