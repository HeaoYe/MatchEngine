#pragma once

#include <Match/vulkan/renderer.hpp>
#include <MatchEngine/function/render/scene/scene_swap_data.hpp>
#include <MatchEngine/function/render/pass/subpass.hpp>

namespace MatchEngine {
    // 场景渲染器
    class SceneRenderer {
    public:
        SceneRenderer();
        ~SceneRenderer();

        void start();
        void render();

        PointerWrapper<SceneSwapData> getSwapData() { return swap_data.get(); }
    private:
        std::unique_ptr<SceneSwapData> swap_data;

        std::shared_ptr<Match::Renderer> renderer;
        std::vector<std::unique_ptr<Subpass>> subpasses;
    };
}
