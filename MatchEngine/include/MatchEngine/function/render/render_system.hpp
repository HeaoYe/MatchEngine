#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/render/swap_data.hpp>
#include <MatchEngine/function/render/render_framework/renderer.hpp>
#include <Match/vulkan/manager.hpp>

namespace MatchEngine {
    class RenderSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(RenderSystem)
    public:
        void createRenderResource();
        void render();
        void waitRenderDevice();

        uint32_t getMaxMeshInstanceCount() const { return 16384; }

        PointerWrapper<Match::APIManager> getMatchAPIManager() { return manager; }
        PointerWrapper<Match::ResourceFactory> getMatchFactory() { return factory.get(); }
        PointerWrapper<SwapData> getSwapData() { return swap_data.get(); }
        PointerWrapper<Renderer::Renderer> getRenderer() { return renderer.get(); }
    private:
        Match::APIManager *manager;
        std::shared_ptr<Match::ResourceFactory> factory;

        std::unique_ptr<SwapData> swap_data;
        std::unique_ptr<Renderer::Renderer> renderer;
    };
}
