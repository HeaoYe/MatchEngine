#include <MatchEngine/function/timer/timer_system.hpp>

namespace MatchEngine {
    TimerSystem::TimerSystem() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;
    }

    std::shared_ptr<Timer> TimerSystem::createTimer(float time_rate) {
        timers.push_back(std::make_shared<Timer>(time_rate));
        return timers.back();
    }

    void TimerSystem::setTimeRateAll(float time_rate) {
        for (auto timer : timers) {
            timer->setTimeRate(time_rate);
        }
    }
    
    void TimerSystem::suspendAll() {
        for (auto timer : timers) {
            timer->suspend();
        }
    }
    
    void TimerSystem::resumeAll() {
        for (auto timer : timers) {
            timer->resume();
        }
    }

    TimerSystem::~TimerSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        timers.clear();
    }
}
