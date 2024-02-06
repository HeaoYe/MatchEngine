#include <MatchEngine/game_framework/scene.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine::Game {
    Scene::~Scene() {
        unload();
        levels.clear();
    }

    PointerWrapper<Level> Scene::createLevel(const std::string &level_name) {
        if (levels.find(level_name) != levels.end()) {
            MCH_CORE_ERROR("Level {} has been created in Scene {}.", level_name, this->name);
            return nullptr;
        }
        if (default_level_name.empty()) {
            default_level_name = level_name;
        }
        levels[level_name] = std::make_unique<Level>(level_name);
        return levels.at(level_name).get();
    }

    void Scene::setDefaultLevel(PointerWrapper<Level> level) {
        setDefaultLevel(level->getLevelName());
    }
    void Scene::setDefaultLevel(const std::string &name) {
        if (levels.find(name) == levels.end()) {
            MCH_CORE_ERROR("Level {} hasn't been appended in Scene {}.", name, this->name);
            return;
        }
        default_level_name = name;
    }

    void Scene::loadLevel(Level *level) {
        loadLevel(level->getLevelName());
    }

    void Scene::loadLevel(const std::string &name) {
        for (auto *active_level : active_levels) {
            if (active_level->getLevelName() == name) {
                MCH_CORE_ERROR("Level {} has been loaded in Scene {}.", name, this->name)
                return;
            }
        }
        active_levels.push_back(levels.at(name).get());
        active_levels.back()->master = this;
        active_levels.back()->load();
    }

    void Scene::unloadLevel(Level *level) {
        unloadLevel(level->getLevelName());
    }

    void Scene::unloadLevel(const std::string &name) {
        for (auto *active_level : active_levels) {
            if (active_level->getLevelName() == name) {
                active_level->unload();
                active_level->master = nullptr;
                active_levels.remove(active_level);
                return;
            }
        }
        MCH_CORE_ERROR("Level {} hasn been unloaded in Scene {}.", name, this->name)
    }

    void Scene::load() {
        game_object_resource_container.load();
        loadLevel(default_level_name);
    }

    void Scene::unload() {
        while (!active_levels.empty()) {
            unloadLevel(active_levels.front());
        }
        game_object_resource_container.unload();
    }

    void Scene::tick(float dt) {
        for (auto *active_level : active_levels) {
            active_level->tick(dt);
        }
    }
}
