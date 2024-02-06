#pragma once

#include <MatchEngine/game_framework/game_object/game_object_resource.hpp>

namespace MatchEngine::Game {
    class GameObjectResourceContainer {
        DefaultNoCopyMoveConstruction(GameObjectResourceContainer)
    public:
        virtual ~GameObjectResourceContainer();
    public:
        void attachGameObjectResource(PointerWrapper<GameObjectResource> resource);
        void detachGameObjectResource(const std::string &resource_name);

        bool hasGameObjectResource(const std::string &resource_name);
        GameObject *createGameObject(const std::string &resource_name);
        GameObject *forceCreateGameObject(const std::string &resource_name);

        void load();
        void unload();
    protected:
        std::map<std::string, PointerWrapper<GameObjectResource>> game_object_resources;
        bool isLoaded { false };
    };
}
