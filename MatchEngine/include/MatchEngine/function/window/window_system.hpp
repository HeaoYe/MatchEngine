#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <Match/vulkan/manager.hpp>

namespace MatchEngine {
    // 窗口系统
    class WindowSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(WindowSystem)
        friend class RenderSystem;
    public:
        void pollEvents();
        bool isAlive();
    private:
        Match::APIManager *api_manager;
    };
}
