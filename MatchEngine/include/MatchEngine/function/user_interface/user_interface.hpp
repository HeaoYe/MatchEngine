#pragma once

#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/core/reflect/reflect_system.hpp>
#include <MatchEngine/core/logger/logger_system.hpp>
#include <MatchEngine/function/input/input_system.hpp>
#include <MatchEngine/function/event/event_system.hpp>
#include <MatchEngine/function/game_framework/component_type_uuid_system.hpp>
#include <MatchEngine/function/resource/resource_system.hpp>

namespace MatchEngine::UserInterface {
    // 用户接口
    // 提供给用户使用的运行时系统
    // extern PointerWrapper<ReflectSystem> reflect;   // declare in MatchEngine/core/reflect/reflect_system.hpp
    extern PointerWrapper<LoggerSystem> logger_system;
    extern PointerWrapper<InputSystem> input;
    extern PointerWrapper<EventSystem> event_system;
    // extern PointerWrapper<ComponentTypeUUIDSystem> component_type_uuid_system;   // declare in MatchEngine/function/game_framework/component_type_uuid_system.hpp
    extern PointerWrapper<ResourceSystem> resource_system;
}
