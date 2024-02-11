#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <Match/vulkan/renderer.hpp>

namespace MatchEngine {
    // TODO: 暂时弃用, 不支持多RenderTarget
    enum class RenderTargetFormat {
        eRGBA8float,
        eRGBA32float,
    };

    // TODO: 暂时弃用, 不支持多RenderTarget
    class RenderTarget {
        NoCopyMoveConstruction(RenderTarget)
        friend class RenderTargetPool;
    public:
        RenderTarget(RenderTargetFormat format, uint32_t width, uint32_t height);
        void setClearValue(float r, float b, float g, float a);
        ~RenderTarget();
    private:
        std::shared_ptr<Match::StorageImage> image;
        vk::ClearColorValue clear_value;
    };

    using RenderTargetID = uint32_t;
}
