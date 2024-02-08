#pragma once

#include <Match/vulkan/renderer.hpp>
#include <MatchEngine/function/render/mesh/mesh_pool.hpp>
#include <MatchEngine/function/render/render_target/render_target_pool.hpp>
#include <MatchEngine/function/render/scene/scene_draw_call_queue.hpp>
#include <MatchEngine/game_framework/component/camera/camera_component.hpp>

namespace MatchEngine {
    // 场景渲染器
    class SceneRenderer {
    public:
        SceneRenderer(const std::string &name);
        ~SceneRenderer();

        void start();
        void addDrawCall(Game::CameraComponent *camera, RenderTargetID render_target_id);
        void render();

        PointerWrapper<MeshPool> getMeshPool() { return mesh_pool.get(); }
        PointerWrapper<RenderTargetPool> getRenderTargetPool() { return render_target_pool.get(); }
    private:
        std::string name;
        std::unique_ptr<MeshPool> mesh_pool;
        std::unique_ptr<RenderTargetPool> render_target_pool;
        std::vector<std::unique_ptr<SceneDrawCallQueue>> draw_call_queues;

        std::shared_ptr<Match::Renderer> renderer;
        std::shared_ptr<Match::GraphicsShaderProgram> mesh_shader_program;
        std::shared_ptr<Match::UniformBuffer> draw_call_data_uniform_buffer;
        std::shared_ptr<Match::DescriptorSet> mesh_shader_program_decriptor_set;
        std::shared_ptr<Match::GraphicsShaderProgram> output_shader_program;
    };
}
