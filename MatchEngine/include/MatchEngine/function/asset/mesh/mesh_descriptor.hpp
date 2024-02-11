#pragma once

#include <MatchEngine/function/asset/mesh/primitive_descriptor.hpp>

namespace MatchEngine {
    // 最大支持的Lod层次数
    constexpr size_t max_level_of_details = 6;

    // Mesh描述符, 指定Mesh有几个Lod层级, 每个Lod对应模型的ID, 已经球体包围盒的半径
    // 6各层次加上其他属性, Mesh描述符刚好是32字节
    struct MeshDescriptor {
        alignas(4) uint32_t lod_count;
        alignas(4) PrimitiveID lods[max_level_of_details];
        alignas(4) float radius;
    };
}
