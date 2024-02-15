#pragma once

#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/render/mesh/mesh_instance_pool.hpp>
#include <Match/vulkan/descriptor_resource/uniform.hpp>

namespace MatchEngine {
    // 场景交换数据, 用于和GameObject交换需要渲染的数据
    class SwapData {
        NoCopyMoveConstruction(SwapData)
    public:
        SwapData();
        ~SwapData();

        void uploadCameraViewMatrix(const glm::mat4 &view);
        void uploadCameraProjectMatrix(const glm::mat4 &project);
        void clear();

        PointerWrapper<MeshInstancePool> getMeshInstancePool() { return mesh_instance_pool.get(); }
        std::shared_ptr<Match::UniformBuffer> getCameraUniformBuffer() { return camera_uniform_buffer; };
    private:
        std::unique_ptr<MeshInstancePool> mesh_instance_pool;
        std::shared_ptr<Match::UniformBuffer> camera_uniform_buffer;
    };
}
