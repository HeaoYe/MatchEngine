#pragma once

#include <MatchEngine/game_framework/game_object.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>

namespace MatchEngine::Game {
    // 场景, 管理GameObject
    class Scene final {
        NoCopyMoveConstruction(Scene)
    public:
        Scene(const std::string &name) : name(name) {}
        ~Scene();

        void start();
        void fixedTick();
        void tick(float dt);
        void postTick(float dt);

        PointerWrapper<GameObject> createGameObject(const std::string &name);
        void removeGameObject(GameObject *game_object);
        void removeGameObject(GameObjectUUID uuid);

        std::string getSceneName() { return name; }
    private:
        std::string name;
        std::map<uint64_t, GameObject *> game_object_map;
        std::list<GameObject *> game_objects;
    };
}
