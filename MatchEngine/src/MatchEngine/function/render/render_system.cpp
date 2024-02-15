#include <MatchEngine/function/render/render_system.hpp>
#include "internal.hpp"

namespace MatchEngine {
    RenderSystem::RenderSystem() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;

        manager = global_runtime_context->window_system->api_manager;
        factory = manager->create_resource_factory("assets");
    }

    void RenderSystem::createRenderResource() {
        swap_data = std::make_unique<SwapData>();
        renderer = std::make_unique<Renderer::Renderer>();
    }

    void RenderSystem::render() {
        renderer->render();
    }
    
    void RenderSystem::waitRenderDevice() {
        vkDeviceWaitIdle(manager->device->device);
    }
    
    RenderSystem::~RenderSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        renderer.reset();
        swap_data.reset();
        
        factory.reset();
        manager = nullptr;
    }
}
