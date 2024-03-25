#pragma once

#include <thread>
#include <chrono>

namespace MatchEngine::Core {
    inline static void Sleep(size_t milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}
