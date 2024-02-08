#pragma once

#include <cstdint>

namespace MatchEngine {
    // Mesh 三角网格
    struct Mesh {
        uint32_t first_instance;
        uint32_t max_instance_count;

        uint32_t vertex_offset;
        uint32_t first_index;
        uint32_t index_count;
    };

    // MeshID
    using MeshID = uint32_t;
}
