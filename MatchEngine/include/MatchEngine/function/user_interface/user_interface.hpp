#pragma once

#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/core/reflect/reflect_system.hpp>
#include <MatchEngine/core/logger/logger_system.hpp>
#include <MatchEngine/function/event/event_system.hpp>
#include <MatchEngine/function/input/input_system.hpp>
#include <MatchEngine/function/timer/timer_system.hpp>
#include <MatchEngine/function/game_framework/scene_manager.hpp>
#include <MatchEngine/function/asset/asset_system.hpp>

namespace MatchEngine::UserInterface {
    // 用户接口
    // 提供给用户使用的运行时系统
    // extern PointerWrapper<ReflectSystem> reflect;   // declare in MatchEngine/core/reflect/reflect_system.hpp
    extern PointerWrapper<LoggerSystem> logger_system;
    extern PointerWrapper<EventSystem> event_system;
    extern PointerWrapper<InputSystem> input;
    extern PointerWrapper<TimerSystem> timer_system;
    extern PointerWrapper<SceneManager> scene_manager;
    extern PointerWrapper<AssetSystem> assets_system;
}
