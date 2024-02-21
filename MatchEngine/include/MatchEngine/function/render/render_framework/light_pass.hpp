#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>

namespace MatchEngine::Renderer {
    // 裁剪光源的Subpass
    class LightPass : public Subpass {
        NoCopyMoveConstruction(LightPass)
    public:
        LightPass() : Subpass("light pass", true) {}

        void postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
        void executePreRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
    private:
        std::shared_ptr<Match::PushConstants> constants;
        std::shared_ptr<Match::DescriptorSet> descriptor_set;
        std::shared_ptr<Match::ComputeShaderProgram> clear_data_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> compute_tile_depth_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> compute_tile_point_light_shader_program;
    };
}
