#pragma once

#include <MatchEngine/function/render/render_target/render_target.hpp>

namespace MatchEngine {
    // TODO: 暂时弃用, 不支持多RenderTarget
    class RenderTargetPool {
        NoCopyMoveConstruction(RenderTargetPool)
        friend class SceneRenderer;
    public:
        RenderTargetPool();
        ~RenderTargetPool();
        RenderTargetID createRenderTarget(RenderTargetFormat format, uint32_t width = -1u, uint32_t height = -1u);
    private:
        std::map<RenderTargetID, std::unique_ptr<RenderTarget>> render_targets;
    };
}
