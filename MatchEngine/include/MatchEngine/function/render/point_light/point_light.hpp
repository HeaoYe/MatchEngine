#pragma once

#include <glm/vec3.hpp>

namespace MatchEngine {
    // 点光源, 位置/半径/颜色
    struct PointLight {
        alignas(16) glm::vec3 position;
        alignas(4) float radius;
        alignas(16) glm::vec3 color;
        alignas(4) float pad = 0;
    };
}
