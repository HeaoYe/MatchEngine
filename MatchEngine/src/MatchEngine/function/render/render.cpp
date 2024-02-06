#include <MatchEngine/function/render/render_system.hpp>

namespace MatchEngine {
    RenderSystem::RenderSystem() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;
    }
    
    RenderSystem::~RenderSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;
    }
}
