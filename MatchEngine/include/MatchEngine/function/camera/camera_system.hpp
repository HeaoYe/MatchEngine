#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/camera/camera_id.hpp>
#include <Match/vulkan/descriptor_resource/uniform.hpp>

namespace MatchEngine {
    // 相机系统, 管理游戏中的所有相机, 管理裁剪所用的相机, 管理游戏编辑器的相机
    class CameraSystem : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(CameraSystem)
    private:
        struct CameraData {
            alignas(16) glm::mat4 view;
            alignas(16) glm::mat4 project;
            alignas(4) float near;
            alignas(4) float far;
        };
    public:
        CameraID allocateCamera(bool is_editor_camera = false);
        void removeCamera(CameraID id);
        void reportCameraViewMatrix(CameraID id, const glm::mat4 &view, bool is_editor_camera = false);
        void reportCameraProjectMatrix(CameraID id, const glm::mat4 &project, bool is_editor_camera = false);
        void reportCameraProjectMatrix(CameraID id, const glm::mat4 &project, float near, float far, bool is_editor_camera = false);
        void reportCameraAsPrimaryViewport(CameraID id);
        void turnOnFixedClip();
        void turnOffFixedClip();

        void activeEditorCamera(CameraID id);
        void deactiveEditorCamera();

        ConstPointerWrapper<CameraData> quertyCameraData(CameraID id) const { return &cameras.at(id); }
        CameraID quertyPrimaryCameraID() const { return current_primary_viewport; };

        std::shared_ptr<Match::UniformBuffer> getViewportCamera() { return viewport_camera; }
        std::shared_ptr<Match::UniformBuffer> getClipCamera() { return clip_camera; }
        bool isFixedClip() const { return fixed_clip; }
    private:
        std::map<CameraID, CameraData> cameras;
        CameraID current_camera_id;
        std::shared_ptr<Match::UniformBuffer> viewport_camera;
        std::shared_ptr<Match::UniformBuffer> clip_camera;
        CameraID current_primary_viewport;
        bool fixed_clip;
        bool editor_camera_active;
    };
}
