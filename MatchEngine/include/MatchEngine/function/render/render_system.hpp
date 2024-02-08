#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/render/scene/scene_renderer.hpp>
#include <Match/vulkan/resource/resource_factory.hpp>

namespace MatchEngine {
    class RenderSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(RenderSystem)
    public:
        void createActiveSceneRenderer();
        void postActiveSceneStart();
        void render();
        void destoryActiveSceneRenderer();

        PointerWrapper<SceneRenderer> getActiveSceneRenderer() { return active_renderer; }
        PointerWrapper<Match::ResourceFactory> getMatchFactory() { return factory.get(); }
    private:
        SceneRenderer *active_renderer;
        std::shared_ptr<Match::ResourceFactory> factory;
    };
}
