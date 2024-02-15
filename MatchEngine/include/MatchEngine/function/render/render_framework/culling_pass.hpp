#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>

namespace MatchEngine::Renderer {
    // 剔除管线
    class CullingPass : public Subpass {
        NoCopyMoveConstruction(CullingPass)
    public:
        CullingPass() : Subpass("culling pass") {}

        void createRenderResource(Match::RenderPassBuilder &builder) override;
        void postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
        void executePreRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
    private:
        std::shared_ptr<Match::DescriptorSet> depth_descriptor_set;
        std::shared_ptr<Match::PushConstants> depth_constants;
        std::shared_ptr<Match::ComputeShaderProgram> copy_depth_texture_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> generate_depth_texture_shader_program;

        std::shared_ptr<Match::DescriptorSet> readonly_descriptor_set;
        std::shared_ptr<Match::DescriptorSet> descriptor_set;
        std::shared_ptr<Match::PushConstants> constants;

        std::shared_ptr<Match::ComputeShaderProgram> pre_culling_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> reset_indirect_command_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> compact_instance_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> generate_available_indirect_command_shader_program;
    };
}
