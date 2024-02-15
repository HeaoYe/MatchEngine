#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace MatchEngine::Game {
    struct color3 : public glm::vec3 {};
    struct color4 : public glm::vec3 {};
    constexpr float fixed_tick_delta_time = 1.0 / 30.0;
}
