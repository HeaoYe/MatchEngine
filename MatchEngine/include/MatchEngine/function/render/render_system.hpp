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

        // 最大MeshInstance数
        uint32_t getMaxMeshInstanceCount() const{ return 16384; }
        // 最大点光源数
        uint32_t getMaxPointLightCount() const { return 1024; }

        PointerWrapper<Match::APIManager> getMatchAPIManager() { return manager; }
        PointerWrapper<Match::ResourceFactory> getMatchFactory() { return factory.get(); }
        std::string getOutputAttachmentName() const { return output_attachment_name; }
        PointerWrapper<SwapData> getSwapData() { return swap_data.get(); }
        PointerWrapper<Renderer::Renderer> getRenderer() { return renderer.get(); }
    private:
        Match::APIManager *manager;
        std::shared_ptr<Match::ResourceFactory> factory;
    INNER_VISIBLE:
        std::string output_attachment_name;
    private:
        std::unique_ptr<SwapData> swap_data;
        std::unique_ptr<Renderer::Renderer> renderer;
    };
}
