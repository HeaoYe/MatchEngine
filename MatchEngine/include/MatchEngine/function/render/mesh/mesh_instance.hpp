#pragma once

#include <MatchEngine/function/asset/mesh/mesh_id.hpp>
#include <glm/vec3.hpp>

namespace MatchEngine {
    // Mesh的实例
    struct MeshInstance {
        alignas(16) glm::vec3 location;
        alignas(16) glm::vec3 rotation;
        alignas(16) glm::vec3 scale;
        alignas(4) MeshID mesh_id;
    };
}
