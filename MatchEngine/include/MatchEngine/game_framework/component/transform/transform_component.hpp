#pragma once

#include <MatchEngine/game_framework/component/component.hpp>
#include <glm/vec3.hpp>

namespace MatchEngine::Game {
    // Transform组件 存储 位置\旋转\缩放 信息
    REFLECT_CLASS(NAME = "TransformComponent")
    class TransformComponent : public Component {
        DECLARE_COMPONENT(TransformComponent)
    public:
        REFLECT_MEMBER()
        glm::vec3 location { 0, 0, 0 };

        REFLECT_MEMBER()
        glm::vec3 rotation { 0, 0, 0 };

        REFLECT_MEMBER()
        glm::vec3 scale { 1, 1, 1 };
    };
};
