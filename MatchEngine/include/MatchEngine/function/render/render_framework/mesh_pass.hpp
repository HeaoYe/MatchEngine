#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>

namespace MatchEngine::Renderer {
    // Mesh渲染管线
    class MeshPass final : public Subpass {
        NoCopyMoveConstruction(MeshPass)
    public:
        MeshPass() : Subpass("mesh pass") {}

        void postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
        void buildPassDescriptor(Match::SubpassBuilder &builder) override;
        void executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
    private:
        std::shared_ptr<Match::DescriptorSet> mesh_shader_program_descriptor_set;
        std::shared_ptr<Match::GraphicsShaderProgram> mesh_shader_program;
    };
}
