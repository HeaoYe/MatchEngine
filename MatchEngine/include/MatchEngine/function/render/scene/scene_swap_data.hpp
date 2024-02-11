#pragma once

#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/render/render_target/render_target_pool.hpp>
#include <MatchEngine/function/render/mesh/mesh_instance_pool.hpp>

namespace MatchEngine {
    // 场景交换数据, 用于和GameObject交换需要渲染的数据
    class SceneSwapData {
        NoCopyMoveConstruction(SceneSwapData)
    public:
        SceneSwapData();
        ~SceneSwapData();

        void uploadCameraViewMatrix(const glm::mat4 &view);
        void uploadCameraProjectMatrix(const glm::mat4 &project);

        // TODO: 暂时弃用, 不支持多RenderTarget
        // PointerWrapper<RenderTargetPool> getRenderTargetPool() { return render_target_pool.get(); }
        PointerWrapper<MeshInstancePool> getMeshInstancePool() { return mesh_instance_pool.get(); }
        std::shared_ptr<Match::UniformBuffer> getCameraUniformBuffer() { return camera_uniform_buffer; };
    private:
        // TODO: 暂时弃用, 不支持多RenderTarget
        // std::unique_ptr<RenderTargetPool> render_target_pool;
        std::unique_ptr<MeshInstancePool> mesh_instance_pool;
        std::shared_ptr<Match::UniformBuffer> camera_uniform_buffer;
    };
}
