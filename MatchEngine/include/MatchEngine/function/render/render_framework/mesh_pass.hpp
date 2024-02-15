#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>

namespace MatchEngine::Renderer {
    // 渲染Mesh的Subpass
    class MeshPass final : public Subpass {
        NoCopyMoveConstruction(MeshPass)
    public:
        MeshPass() : Subpass("mesh pass") {}
        ~MeshPass() override;

        void createRenderResource(Match::RenderPassBuilder &builder, Resource &resource) override;
        void postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
        void buildPassDescriptor(Match::SubpassBuilder &builder) override;
        void executePreRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
        void executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
    private:
        std::vector<vk::CommandBuffer> command_buffers;
    private:
        std::vector<vk::Fence> compute_fences;

        std::shared_ptr<Match::DescriptorSet> compute_shader_program_descriptor_set;
        std::shared_ptr<Match::PushConstants> compute_shader_constants;
        std::shared_ptr<Match::ComputeShaderProgram> collect_mesh_instance_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> generate_indirect_command_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> compact_instance_data_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> collect_available_indirect_command_shader_program;

        std::shared_ptr<Match::DescriptorSet> mesh_shader_program_descriptor_set;
        std::shared_ptr<Match::GraphicsShaderProgram> mesh_pre_z_shader_program;
        std::shared_ptr<Match::GraphicsShaderProgram> mesh_shader_program;
    };
}
