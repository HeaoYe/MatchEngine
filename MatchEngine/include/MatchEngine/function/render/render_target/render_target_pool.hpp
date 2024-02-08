#pragma once

#include <MatchEngine/function/render/render_target/render_target.hpp>

namespace MatchEngine {
    // 渲染目标池, 管理一个场景中的所有渲染目标
    class RenderTargetPool {
        NoCopyMoveConstruction(RenderTargetPool)
        friend class SceneRenderer;
    public:
        RenderTargetPool(uint32_t width, uint32_t height);
        ~RenderTargetPool();
        RenderTargetID createRenderTarget(RenderTargetFormat format);
        void setOutputRenderTarget(RenderTargetID id);
        void clearRenderTarget(RenderTargetID id);
    private:
        void build();
    private:
        bool built;
        uint32_t width, height;
        RenderTargetID output_render_target_id;
        std::vector<RenderTargetID> exist_render_targets;
        std::map<RenderTargetID, std::unique_ptr<RenderTarget>> render_targets;

        std::shared_ptr<Match::Sampler> sampler;
        std::shared_ptr<Match::Texture> empty_texture;
        std::shared_ptr<Match::DescriptorSet> render_target_descriptor_set;
    };
}
