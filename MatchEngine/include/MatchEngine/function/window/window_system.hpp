#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>

namespace MatchEngine {
    // 窗口系统
    class WindowSystem final : public RuntimeSystem {
    public:
        WindowSystem();
        ~WindowSystem() override;

        void pollEvents();
        bool isAlive();
      
        virtual std::string getSystemName() const override { return "WindowSystem"; }
    };
}
