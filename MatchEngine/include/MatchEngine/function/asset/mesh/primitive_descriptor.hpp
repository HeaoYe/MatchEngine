#pragma once

#include <cstdint>
#include <glm/vec3.hpp>

namespace MatchEngine {
    // Primitive描述符, 描述用于绘制Primitive的信息, pad没有用处, 为保持PrimitiveDescriptor为32字节
    struct PrimitiveDescriptor {
        alignas(4) int32_t vertex_offset;
        alignas(4) uint32_t first_index;
        alignas(4) uint32_t index_count;
        alignas(4) uint32_t pad;
    };

    // PrimitiveID
    using PrimitiveID = uint32_t;
}
