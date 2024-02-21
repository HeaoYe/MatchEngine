#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>

namespace MatchEngine::Renderer {
    // 写入Visibility Buffer的管线
    // Visibility Buffer是一个Uint64 Buffer，前32位存储对应位置的 实例索引，后32位存储 三角形索引
    // 根据两个索引就可以检索到对应的三角形顶点信息
    // 由于每一帧的实例索引都是由计算着色器实时生成的，所以VisibilityBuffer的颜色在闪
    class VisibilityPass : public Subpass {
        NoCopyMoveConstruction(VisibilityPass)
    public:
        VisibilityPass() : Subpass("visibility pass") {}

        void createRenderResource(Match::RenderPassBuilder &builder) override;
        void buildPassDescriptor(Match::SubpassBuilder &builder) override;
        void postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
        void executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) override;
    private:
        std::shared_ptr<Match::DescriptorSet> descriptor_set;
        std::shared_ptr<Match::GraphicsShaderProgram> visibility_buffer_shader_program;
    };
}
