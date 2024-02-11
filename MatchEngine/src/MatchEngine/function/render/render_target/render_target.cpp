#include <MatchEngine/function/render/render_target/render_target.hpp>
#include "internal.hpp"

namespace MatchEngine {
    RenderTarget::RenderTarget(RenderTargetFormat format, uint32_t width, uint32_t height) {
        vk::Format vk_format;
        switch (format) {
        case RenderTargetFormat::eRGBA8float:
            vk_format = vk::Format::eR8G8B8A8Snorm;
            break;
        case RenderTargetFormat::eRGBA32float:
            vk_format = vk::Format::eR32G32B32A32Sfloat;
            break;
        }
        image = global_runtime_context->render_system->getMatchFactory()->create_storage_image(width, height, vk_format, true, true);
        clear_value = { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    void RenderTarget::setClearValue(float r, float b, float g, float a) {
        clear_value = { r, g, b, a };
    }

    RenderTarget::~RenderTarget() {
        image.reset();
    }
}
