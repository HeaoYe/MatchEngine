#pragma once

#include <MatchEngine/core/base/singleton.hpp>
#include <MatchEngine/core/reflect/reflect_system.hpp>
#include <MatchEngine/core/logger/logger_system.hpp>
#include <MatchEngine/function/window/window_system.hpp>
#include <MatchEngine/function/input/input_system.hpp>
#include <MatchEngine/function/event/event_system.hpp>
#include <MatchEngine/function/game_framework/game_object_uuid_allocator.hpp>
#include <MatchEngine/function/game_framework/component_type_uuid_system.hpp>
#include <MatchEngine/function/game_framework/scene_manager.hpp>
#include <MatchEngine/function/render/render_system.hpp>
#include <MatchEngine/function/asset/asset_system.hpp>

namespace MatchEngine {
    // 管理全部运行时系统
    struct RuntimeContext {
        Singleton<ReflectSystem> reflect_system;
        Singleton<LoggerSystem> logger_system;
        Singleton<WindowSystem> window_system;
        Singleton<InputSystem> input_system;
        Singleton<EventSystem> event_system;
        Singleton<GameObjectUUIDAllocator> game_object_uuid_allocator;
        Singleton<ComponentTypeUUIDSystem> component_type_uuid_system;
        Singleton<SceneManager> scene_manager;
        Singleton<RenderSystem> render_system;
        Singleton<AssetSystem> assets_system;
    };
}
