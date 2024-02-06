#pragma once

#include <MatchEngine/game_framework/component/declare_component.hpp>
#include <glm/vec3.hpp>

namespace MatchEngine::Game {
    REFLECT_CLASS(NAME = "LocationComponent")
    class LocationComponent : public Component {
        DECLARE_COMPONENT(LocationComponent)
        DECLARE_COMPONENT_OVERRIDES()
        ALLOW_REFLECT_PRIVATE()
    private:
        REFLECT_MEMBER()
        glm::vec3 location;
    };
};
