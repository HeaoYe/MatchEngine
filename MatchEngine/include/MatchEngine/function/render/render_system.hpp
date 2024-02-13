#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/render/scene/scene_renderer.hpp>
#include <Match/vulkan/resource/resource_factory.hpp>
#include <Match/constant.hpp>

namespace MatchEngine {
    class RenderSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(RenderSystem)
    public:
        void createActiveSceneRenderer();
        void postActiveSceneStart();
        void render();
        void destoryActiveSceneRenderer();
        void waitRenderDevice();

        PointerWrapper<SceneRenderer> getActiveSceneRenderer() { return active_renderer; }
        PointerWrapper<Match::ResourceFactory> getMatchFactory() { return factory.get(); }
        std::string getOutputAttachmentName() {
            return output_attachment_name;
        }
    MATCHEDITOR_VISIBLE:
        std::string output_attachment_name = Match::SWAPCHAIN_IMAGE_ATTACHMENT;
    private:
        SceneRenderer *active_renderer;
        std::shared_ptr<Match::ResourceFactory> factory;
    };
}
