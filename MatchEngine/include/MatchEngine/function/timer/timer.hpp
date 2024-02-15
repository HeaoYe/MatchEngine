#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <chrono>

namespace MatchEngine {
    // 定时器，可以通过tick()以指定delta_time_ms更新，并返回真正的delta_time(秒)，可以改变时间速率，可以暂停和恢复时间
    class Timer {
        NoCopyMoveConstruction(Timer);
    public:
        Timer(float time_rate);
        ~Timer();
        float tick(std::chrono::milliseconds delta_time_ms = std::chrono::milliseconds(0));
        void setTimeRate(float time_rate);
        void suspend();
        void resume();
        bool is_suspended() const { return suspended; }
    private:
        std::chrono::high_resolution_clock clock;
        std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
        float time_rate;
        bool suspended;
    };
}
