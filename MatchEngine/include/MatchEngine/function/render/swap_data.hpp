#pragma once

#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/render/mesh/mesh_instance_pool.hpp>
#include <MatchEngine/function/render/point_light/point_light_pool.hpp>
#include <Match/vulkan/descriptor_resource/uniform.hpp>

namespace MatchEngine {
    // 场景交换数据, 用于和GameObject交换需要渲染的数据
    class SwapData {
        NoCopyMoveConstruction(SwapData)
    public:
        SwapData();
        ~SwapData();

        void clear();

        PointerWrapper<MeshInstancePool> getMeshInstancePool() { return mesh_instance_pool.get(); }
        PointerWrapper<PointLightPool> getPointLightPool() { return point_light_pool.get(); }
    private:
        std::unique_ptr<MeshInstancePool> mesh_instance_pool;
        std::unique_ptr<PointLightPool> point_light_pool;
    };
}
