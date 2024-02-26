#pragma once

#include <glm/glm.hpp>

namespace MatchEngine::Core {
    /**
     * @brief 32位浮点数向量
     *
     * TODO: 不依赖glm实现
     */
    using Vectorf2 = glm::vec2;
    using Vectorf3 = glm::vec3;
    using Vectorf4 = glm::vec4;

    /**
     * @brief 64位浮点数向量
     *
     * TODO: 不依赖glm实现
     */
    using Vectord2 = glm::dvec2;
    using Vectord3 = glm::dvec3;
    using Vectord4 = glm::dvec4;

    /**
     * @brief 32位整数向量
     *
     * TODO: 不依赖glm实现
     */
    using Vectori2 = glm::ivec2;
    using Vectori3 = glm::ivec3;
    using Vectori4 = glm::ivec4;

    /**
     * @brief 默认向量定义
     *
     */
    using Vector2 = Vectorf2;
    using Vector3 = Vectorf3;
    using Vector4 = Vectorf4;
}
