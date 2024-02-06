#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>

namespace MatchEngine {
    // 窗口系统
    class WindowSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(WindowSystem)
    public:
        void pollEvents();
        bool isAlive();
    };
}
