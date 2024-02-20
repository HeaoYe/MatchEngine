#include <MatchEngine/function/camera/camera_system.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "internal.hpp"

namespace MatchEngine {
    CameraSystem::CameraSystem() {
        initializeRuntimeSystem();

        current_camera_id = 0;
        current_primary_viewport = 0;
        fixed_clip = false;

        auto factory = global_runtime_context->render_system->getMatchFactory();
        viewport_camera = factory->create_uniform_buffer(sizeof(CameraData));
        clip_camera = factory->create_uniform_buffer(sizeof(CameraData));

        state = RuntimeSystem::State::eInitialized;
    }

    CameraSystem::~CameraSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;
    }

    CameraID CameraSystem::allocateCamera(bool is_editor_camera) {
        current_camera_id ++;
        cameras.insert(std::make_pair(current_camera_id, CameraData { .view = glm::mat4(1), .project = glm::ortho<float>(0, 1, 0, 1, 0, 1) }));
        if ((current_primary_viewport == 0) && (!is_editor_camera)) {
            reportCameraAsPrimaryViewport(current_camera_id);
        }
        return current_camera_id;
    }

    void CameraSystem::removeCamera(CameraID id) {
        cameras.erase(id);
    }

    void CameraSystem::reportCameraViewMatrix(CameraID id, const glm::mat4 &view, bool is_editor_camera) {
        cameras.at(id).view = view;
        if (editor_camera_active && (!is_editor_camera)) {
            return;
        }
        if ((id == current_primary_viewport) || is_editor_camera) {
            static_cast<CameraData *>(viewport_camera->get_uniform_ptr())->view = view;
            if (!fixed_clip) {
                memcpy(clip_camera->get_uniform_ptr(), &cameras.at(id).view, sizeof(glm::mat4));
            }
        }
    }
    
    void CameraSystem::reportCameraProjectMatrix(CameraID id, const glm::mat4 &project, bool is_editor_camera) {
        float near = project[3][2] / (project[2][2] - 1);
        float far = project[3][2] / (project[2][2] + 1);
        reportCameraProjectMatrix(id, project, near, far, is_editor_camera);
    }

    void CameraSystem::reportCameraProjectMatrix(CameraID id, const glm::mat4 &project, float near, float far, bool is_editor_camera) {
        auto &camera_data = cameras.at(id);
        camera_data.project = project;
        camera_data.near = near;
        camera_data.far = far;
        if (editor_camera_active && (!is_editor_camera)) {
            return;
        }
        if ((id == current_primary_viewport) || is_editor_camera) {
            auto data_ptr = static_cast<CameraData *>(viewport_camera->get_uniform_ptr());
            data_ptr->project = project;
            data_ptr->near = near;
            data_ptr->far = far;
            if (!fixed_clip) {
                data_ptr = static_cast<CameraData *>(clip_camera->get_uniform_ptr());
                data_ptr->project = project;
                data_ptr->near = near;
                data_ptr->far = far;
            }
        }
    }

    void CameraSystem::reportCameraAsPrimaryViewport(CameraID id) {
        current_primary_viewport = id;
        if (editor_camera_active) {
            return;
        }
        memcpy(viewport_camera->get_uniform_ptr(), &cameras.at(id), sizeof(CameraData));
        if (!fixed_clip) {
            memcpy(clip_camera->get_uniform_ptr(), &cameras.at(id), sizeof(CameraData));
        }
    }

    void CameraSystem::turnOnFixedClip() {
        fixed_clip = true;
    }

    void CameraSystem::turnOffFixedClip() {
        fixed_clip = false;
        memcpy(clip_camera->get_uniform_ptr(), viewport_camera->get_uniform_ptr(), sizeof(CameraData));
    }
 
    void CameraSystem::activeEditorCamera(CameraID id) {
        editor_camera_active = true;

        memcpy(viewport_camera->get_uniform_ptr(), &cameras.at(id), sizeof(CameraData));
        if (!fixed_clip) {
            memcpy(clip_camera->get_uniform_ptr(), &cameras.at(id), sizeof(CameraData));
        }
    }

    void CameraSystem::deactiveEditorCamera() {
        editor_camera_active = false;
        reportCameraAsPrimaryViewport(current_primary_viewport);
    }
}
