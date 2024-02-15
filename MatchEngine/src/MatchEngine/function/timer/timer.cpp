#include <MatchEngine/function/timer/timer.hpp>
#include <thread>

namespace MatchEngine {
    Timer::Timer(float time_rate) : time_rate(time_rate) {
        last_time = clock.now();
        suspended = false;
    }
    
    Timer::~Timer() {}
    
    float Timer::tick(std::chrono::milliseconds delta_time_ms) {
        auto dt = std::chrono::duration_cast<std::chrono::microseconds>(clock.now() - last_time).count();
        auto wait = std::chrono::duration_cast<std::chrono::microseconds>(delta_time_ms).count() - dt;
        if (wait > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
            last_time = clock.now();
            return time_rate * static_cast<float>(delta_time_ms.count()) * 0.001f;
        }
        last_time = clock.now();
        return time_rate * static_cast<float>(dt) * 0.000001f;
    }
    
    void Timer::setTimeRate(float time_rate) {
        this->time_rate = time_rate;
    }

    void Timer::suspend() {
        suspended = true;
    }
    
    void Timer::resume() {
        suspended = false;
        last_time = clock.now();
    }
}
