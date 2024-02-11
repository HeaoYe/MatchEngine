#include <MatchEngine/function/render/render_target/render_target_pool.hpp>
#include <Match/core/setting.hpp>

namespace MatchEngine {
    RenderTargetPool::RenderTargetPool() {}

    RenderTargetPool::~RenderTargetPool() {}

    RenderTargetID RenderTargetPool::createRenderTarget(RenderTargetFormat format, uint32_t width, uint32_t height) {
        if (width == -1u) {
            width = Match::runtime_setting->get_window_size().width;
        }
        if (height == -1u) {
            height = Match::runtime_setting->get_window_size().height;
        }

        render_targets.insert(std::make_pair(
            render_targets.size(),
            std::make_unique<RenderTarget>(format, width, height)
        ));

        return render_targets.size() - 1;
    }    
}
