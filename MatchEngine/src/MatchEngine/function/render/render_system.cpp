#include <MatchEngine/function/render/render_system.hpp>
#include "internal.hpp"

namespace MatchEngine {
    RenderSystem::RenderSystem() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;

        factory = global_runtime_context->window_system->getAPIManager()->create_resource_factory("assets");
    }

    void RenderSystem::createActiveSceneRenderer() {
        active_renderer = new SceneRenderer();
    }

    void RenderSystem::postActiveSceneStart() {
        active_renderer->start();
    }

    void RenderSystem::render() {
        active_renderer->render();
    }
    
    void RenderSystem::destoryActiveSceneRenderer() {
        if (active_renderer != nullptr) {
            delete active_renderer;
            active_renderer = nullptr;
        }
    }
    
    void RenderSystem::waitRenderDevice() {
        vkDeviceWaitIdle(global_runtime_context->window_system->getAPIManager()->device->device);
    }
    
    RenderSystem::~RenderSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;
        
        destoryActiveSceneRenderer();

        factory.reset();
    }
}
