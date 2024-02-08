#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <Match/vulkan/renderer.hpp>

namespace MatchEngine {
    // 不同精度的RenderTarget
    enum class RenderTargetFormat {
        eRGBA8float,
        eRGBA32float,
    };

    // 渲染目标, 一般一个相机会有一个渲染目标, 存储相机视角下的图像
    class RenderTarget {
        NoCopyMoveConstruction(RenderTarget)
        friend class RenderTargetPool;
    public:
        RenderTarget(RenderTargetFormat format, uint32_t width, uint32_t height);
        ~RenderTarget();
    private:
        std::shared_ptr<Match::StorageImage> image;
        vk::ClearColorValue clear_value;
    };

    using RenderTargetID = uint32_t;
}
