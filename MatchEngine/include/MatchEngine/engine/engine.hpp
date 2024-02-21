#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/function/timer/timer.hpp>
#include <functional>
#include <memory>
#include <thread>

namespace MatchEngine {
    class MatchEngine {
        DefaultNoCopyMoveConstruction(MatchEngine)
    public:
        bool initialize();
        void destroy();

        void gameLoop();
    MATCHEDITOR_VISIBLE:
        void start();
        void stop();
        void tickLogic();
        void tickRender();
        void tickInputState();
        void waitFixedTickThread();
    private:
        bool checkRuntimeSystem(const RuntimeSystem *system);
    private:
        std::shared_ptr<Timer> main_loop_timer;
        std::shared_ptr<Timer> fixed_tick_timer;
        std::shared_ptr<Timer> benchmark;
        std::unique_ptr<std::thread> fixed_tick_thread;
        float dt;
    MATCHEDITOR_VISIBLE:
        bool editor_mode { false };  // set by MatchEditor
        std::function<void()> hook_imgui_layer;
    };
}
