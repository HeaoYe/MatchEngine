#pragma once

#include <MatchEngine/core/base/singleton.hpp>
#include <MatchEngine/core/reflect/reflect_system.hpp>
#include <MatchEngine/core/logger/logger_system.hpp>
#include <MatchEngine/function/window/window_system.hpp>
#include <MatchEngine/function/input/input_system.hpp>
#include <MatchEngine/function/event/event_system.hpp>

namespace MatchEngine {
    // 管理全部运行时系统
    struct RuntimeContext {
        Singleton<ReflectSystem> reflect_system;
        Singleton<LoggerSystem> logger_system;
        Singleton<WindowSystem> window_system;
        Singleton<InputSystem> input_system;
        Singleton<EventSystem> event_system;
    };
}
