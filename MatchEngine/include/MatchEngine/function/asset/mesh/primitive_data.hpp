#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace MatchEngine {
    // Primitive数据
    struct PrimitiveData {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> tex_coords;
        std::vector<glm::vec3> colors;

        std::vector<uint32_t> indices;
    };

    // Mesh数据, 每层Lod都是一个Primitive
    struct MeshData {
        std::vector<PrimitiveData> lods;
    };
}
