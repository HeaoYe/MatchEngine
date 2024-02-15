#pragma once

#include <MatchEngine/function/asset/mesh/primitive_descriptor.hpp>
#include <glm/vec4.hpp>

namespace MatchEngine {
    // 最大支持的Lod层次数
    constexpr size_t max_level_of_details = 7;

    // Mesh描述符, 指定Mesh有几个Lod层级, 每个Lod对应模型的ID, 以及球体包围盒的半径，以及AABB包围盒
    // 7个层次加上其他属性, Mesh描述符刚好是64字节
    struct MeshDescriptor {
        alignas(4) uint32_t lod_count;
        alignas(4) PrimitiveID lods[max_level_of_details];
        alignas(4) glm::vec3 aabb_min;
        alignas(4) float radius;
        alignas(4) glm::vec3 aabb_max;
        alignas(4) float pad;
    };
}
