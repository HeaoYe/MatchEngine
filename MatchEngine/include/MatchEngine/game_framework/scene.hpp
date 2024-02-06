#pragma once

#include <MatchEngine/game_framework/level.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <memory>

namespace MatchEngine::Game {
    // 场景, 用于管理多个Level, 可以运行时动态的加载和卸载Level
    class Scene final {
        NoCopyMoveConstruction(Scene)
    public:
        Scene(const std::string &name) : name(name) {}
        ~Scene();

        PointerWrapper<Level> createLevel(const std::string &level_name);

        void setDefaultLevel(PointerWrapper<Level> level);
        void setDefaultLevel(const std::string &name);

        void loadLevel(Level *level);
        void loadLevel(const std::string &name);
        void unloadLevel(Level *level);
        void unloadLevel(const std::string &name);

        void load();
        void unload();

        void tick(float dt);

        std::string getSceneName() { return name; }
        GameObjectResourceContainer &getGameObjectResourceContainer() { return game_object_resource_container; }
    private:
        std::string name;
        GameObjectResourceContainer game_object_resource_container;
        std::map<std::string, std::unique_ptr<Level>> levels;
        std::string default_level_name;
        std::list<Level *> active_levels;
    };
}
