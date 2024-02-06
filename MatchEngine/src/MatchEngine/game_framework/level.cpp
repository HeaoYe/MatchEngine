#include <MatchEngine/game_framework/level.hpp>
#include <MatchEngine/game_framework/scene.hpp>

namespace MatchEngine::Game {
    Level::~Level() {}

    void Level::load() {
        game_object_resource_container.load();
    }
    
    void Level::unload() {
        game_object_resource_container.unload();
    }

    void Level::tick(float dt) {
        root_game_object.tick(dt);
    }

    GameObject *Level::createGameObject(const std::string &resource_name) {
        if (game_object_resource_container.hasGameObjectResource(resource_name)) {
            return game_object_resource_container.forceCreateGameObject(resource_name);
        }
        return master->getGameObjectResourceContainer().createGameObject(resource_name);
    }
}
