#include <MatchEngine/function/render/swap_data.hpp>
#include <internal.hpp>

namespace MatchEngine {
    SwapData::SwapData() {
        mesh_instance_pool = std::make_unique<MeshInstancePool>(global_runtime_context->render_system->getMaxMeshInstanceCount());
        camera_uniform_buffer = global_runtime_context->render_system->getMatchFactory()->create_uniform_buffer(sizeof(glm::mat4) * 2 + sizeof(float) * 2);
        scene_camera_uniform_buffer = global_runtime_context->render_system->getMatchFactory()->create_uniform_buffer(sizeof(glm::mat4) * 2 + sizeof(float) * 2);
        camera_fixed = false;
    }

    void SwapData::setCameraFixed(bool fixed) {
        camera_fixed = fixed;
        if (!camera_fixed) {
            memcpy(camera_uniform_buffer->get_uniform_ptr(), scene_camera_uniform_buffer->get_uniform_ptr(), camera_uniform_buffer->size);
        }
    }

    void SwapData::uploadCameraViewMatrix(const glm::mat4 &view) {
        if (!camera_fixed) {
            static_cast<glm::mat4 *>(camera_uniform_buffer->get_uniform_ptr())[0] = view;
        }
        static_cast<glm::mat4 *>(scene_camera_uniform_buffer->get_uniform_ptr())[0] = view;
    }

    void SwapData::uploadCameraProjectMatrix(const glm::mat4 &project) {
        if (!camera_fixed) {
            static_cast<glm::mat4 *>(camera_uniform_buffer->get_uniform_ptr())[1] = project;
        }
        static_cast<glm::mat4 *>(scene_camera_uniform_buffer->get_uniform_ptr())[1] = project;
    }

    void SwapData::uploadCameraNearFar(float near, float far) {
        if (!camera_fixed) {
            static_cast<float *>(camera_uniform_buffer->get_uniform_ptr())[32] = near;
            static_cast<float *>(camera_uniform_buffer->get_uniform_ptr())[33] = far;
        }
        static_cast<float *>(scene_camera_uniform_buffer->get_uniform_ptr())[32] = near;
        static_cast<float *>(scene_camera_uniform_buffer->get_uniform_ptr())[33] = far;
    }

    void SwapData::clear() {
        mesh_instance_pool->clear();
        memset(camera_uniform_buffer->get_uniform_ptr(), 0, camera_uniform_buffer->size);
    }

    SwapData::~SwapData() {
        camera_uniform_buffer.reset();
        mesh_instance_pool.reset();
    }
}
