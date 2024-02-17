#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>

namespace MatchEngine::Renderer {
    class OutliningPass : public Subpass {
        NoCopyMoveConstruction(OutliningPass)
        friend class OutliningPass;
    public:
        OutliningPass() : Subpass("outlining pass") {}

        void buildPassDescriptor(Match::SubpassBuilder &builder) override;
        void postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
        void executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
    private:
        std::shared_ptr<Match::DescriptorSet> descriptor_set;
        std::shared_ptr<Match::GraphicsShaderProgram> stencil_shader_program;
        std::shared_ptr<Match::GraphicsShaderProgram> outlining_shader_program;
    };
}
