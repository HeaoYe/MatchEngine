#pragma once

#include <MatchEngine/function/render/mesh/mesh.hpp>
#include <glm/vec3.hpp>

namespace MatchEngine {
    // Mesh的实例
    struct MeshInstance {
        MeshID mesh_id;

        glm::vec3 location;
        glm::vec3 rotation;
        glm::vec3 scale;
    };
}
