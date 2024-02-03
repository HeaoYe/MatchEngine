#pragma once

#include <MatchEngine/core/base/singleton.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include <MatchEngine/function/input/event_system.hpp>

namespace MatchEngine {
    // 管理全部运行时系统
    struct RuntimeContext {
        Singleton<LoggerSystem> logger_system;
        Singleton<EventSystem> event_system;
    };
}
