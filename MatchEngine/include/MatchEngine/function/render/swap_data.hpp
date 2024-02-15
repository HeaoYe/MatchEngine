#pragma once

#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/render/mesh/mesh_instance_pool.hpp>
#include <Match/vulkan/descriptor_resource/uniform.hpp>

namespace MatchEngine {
    // 场景交换数据, 用于和GameObject交换需要渲染的数据
    // 目前相机的数据直接写在SwapData中，未来会有专门的相机系统
    // TODO：CameraSystem
    class SwapData {
        NoCopyMoveConstruction(SwapData)
    public:
        SwapData();
        ~SwapData();

        void setCameraFixed(bool fixed);
        void uploadCameraViewMatrix(const glm::mat4 &view);
        void uploadCameraProjectMatrix(const glm::mat4 &project);
        void uploadCameraNearFar(float near, float far);

        void clear();

        PointerWrapper<MeshInstancePool> getMeshInstancePool() { return mesh_instance_pool.get(); }
        bool isCameraFixed() const { return camera_fixed; }
        std::shared_ptr<Match::UniformBuffer> getCameraUniformBuffer() { return camera_uniform_buffer; };
        std::shared_ptr<Match::UniformBuffer> getSceneCameraUniformBuffer() { return scene_camera_uniform_buffer; };
    private:
        std::unique_ptr<MeshInstancePool> mesh_instance_pool;
        std::shared_ptr<Match::UniformBuffer> camera_uniform_buffer;
        std::shared_ptr<Match::UniformBuffer> scene_camera_uniform_buffer;
        bool camera_fixed;
    };
}
