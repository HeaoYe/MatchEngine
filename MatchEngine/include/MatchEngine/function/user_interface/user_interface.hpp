#pragma once

#include <MatchEngine/function/user_interface/pointer_wrapper.hpp>
#include <MatchEngine/core/reflect/reflect_system.hpp>
#include <MatchEngine/core/logger/logger_system.hpp>
#include <MatchEngine/function/input/input_system.hpp>
#include <MatchEngine/function/event/event_system.hpp>

namespace MatchEngine::UserInterface {
    // 用户接口
    // 提供给用户使用的运行时系统
    // extern PointerWrapper<ReflectSystem> reflect;   // declare in MatchEngine/core/reflect/reflect_system.hpp
    extern PointerWrapper<LoggerSystem> logger_system;
    extern PointerWrapper<InputSystem> input;
    extern PointerWrapper<EventSystem> event_system;
}
