#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/function/timer/timer.hpp>
#include <vector>
#include <memory>

namespace MatchEngine {
    // 定时器系统，创建和管理所有定时器
    class TimerSystem : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(TimerSystem)
    public:
        std::shared_ptr<Timer> createTimer(float time_rate = 1.0f);
        void setTimeRateAll(float time_rate);
        void suspendAll();
        void resumeAll();
    private:
        std::vector<std::shared_ptr<Timer>> timers;
    };
}
