#pragma once

#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/render/mesh/mesh_instance_pool.hpp>
#include <Match/vulkan/descriptor_resource/uniform.hpp>

namespace MatchEngine {
    // 场景交换数据, 用于和GameObject交换需要渲染的数据
    // 目前相机的数据直接写在SwapData中，未来会有专门的相机系统
    class SwapData {
        NoCopyMoveConstruction(SwapData)
    public:
        SwapData();
        ~SwapData();

        void clear();

        PointerWrapper<MeshInstancePool> getMeshInstancePool() { return mesh_instance_pool.get(); }
    private:
        std::unique_ptr<MeshInstancePool> mesh_instance_pool;
    };
}
