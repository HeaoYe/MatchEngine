#include <MatchEngine/function/render/scene/scene_swap_data.hpp>
#include <internal.hpp>

namespace MatchEngine {
    SceneSwapData::SceneSwapData() {
        mesh_instance_pool = std::make_unique<MeshInstancePool>(16384);
        camera_uniform_buffer = global_runtime_context->render_system->getMatchFactory()->create_uniform_buffer(sizeof(glm::mat4) * 2 + sizeof(glm::vec4) * 6);
    }

    void SceneSwapData::uploadCameraViewMatrix(const glm::mat4 &view) {
        static_cast<glm::mat4 *>(camera_uniform_buffer->get_uniform_ptr())[0] = view;
    }

    void SceneSwapData::uploadCameraProjectMatrix(const glm::mat4 &project) {
        static_cast<glm::mat4 *>(camera_uniform_buffer->get_uniform_ptr())[1] = project;
        auto planes = static_cast<glm::vec4 *>(camera_uniform_buffer->get_uniform_ptr()) + 8;
        // Left
        planes[0].x = { project[0].w + project[0].x };
        planes[0].y = { project[1].w + project[1].x };
        planes[0].z = { project[2].w + project[2].x };
        planes[0].w = { project[3].w + project[3].x };
        // Right
        planes[1].x = { project[0].w - project[0].x };
        planes[1].y = { project[1].w - project[1].x };
        planes[1].z = { project[2].w - project[2].x };
        planes[1].w = { project[3].w - project[3].x };
        // Top
        planes[2].x = { project[0].w + project[0].y };
        planes[2].y = { project[1].w + project[1].y };
        planes[2].z = { project[2].w + project[2].y };
        planes[2].w = { project[3].w + project[3].y };
        // Bottom
        planes[3].x = { project[0].w - project[0].y };
        planes[3].y = { project[1].w - project[1].y };
        planes[3].z = { project[2].w - project[2].y };
        planes[3].w = { project[3].w - project[3].y };
        // Back
        planes[4].x = { project[0].w + project[0].z };
        planes[4].y = { project[1].w + project[1].z };
        planes[4].z = { project[2].w + project[2].z };
        planes[4].w = { project[3].w + project[3].z };
        // Front
        planes[5].x = { project[0].w - project[0].z };
        planes[5].y = { project[1].w - project[1].z };
        planes[5].z = { project[2].w - project[2].z };
        planes[5].w = { project[3].w - project[3].z };

        for (size_t i = 0; i < 6; i ++) {
            planes[i] /= glm::sqrt(planes[i].x * planes[i].x + planes[i].y * planes[i].y + planes[i].z * planes[i].z);
        }
    }

    SceneSwapData::~SceneSwapData() {
        camera_uniform_buffer.reset();
        mesh_instance_pool.reset();
    }
}
