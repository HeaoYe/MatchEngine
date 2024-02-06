#include <MatchEngine/engine/engine.hpp>
#include <MatchEngine/function/user_interface/user_interface.hpp>
#include "internal.hpp"
#include <iostream>

namespace MatchEngine {
    bool MatchEngine::initialize() {
        if (global_runtime_context != nullptr) {
            if (core_logger != nullptr) {
                MCH_CORE_FATAL("Maybe you have initialized a MatchEngine, don't initilize again.")
            } else {
                std::cout << "Maybe you meet some error when initialize a MatchEngine, please destroy it at first and try to initialize again." << std::endl;
            }
            return false;
        }
        global_runtime_context = new RuntimeContext();

        // 加载反射系统
        global_runtime_context->reflect_system.initialize();
        UserInterface::reflect.ptr = global_runtime_context->reflect_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::reflect.ptr)) {
            std::cout << "Failed initialize " << global_runtime_context->reflect_system->getSystemName() << "." << std::endl;
            return false;
        }
        global_runtime_context->reflect_system->registerReflectProperties();

        // 初始化日志系统
        global_runtime_context->logger_system.initialize(Logger::Level::eInfo);
        UserInterface::logger_system.ptr = global_runtime_context->logger_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::logger_system.ptr)) {
            std::cout << "Failed initialize " << global_runtime_context->logger_system->getSystemName() << "." << std::endl;
            return false;
        }

        // 初始化窗口系统
        global_runtime_context->window_system.initialize();
        if (!checkRuntimeSystem(global_runtime_context->window_system.getInstance())) {
            return false;
        }

        // 初始化输入系统
        global_runtime_context->input_system.initialize();
        UserInterface::input.ptr = global_runtime_context->input_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::input.ptr)) {
            return false;
        }

        // 初始化事件系统
        global_runtime_context->event_system.initialize();
        UserInterface::event_system.ptr = global_runtime_context->event_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::event_system.ptr)) {
            return false;
        }

        // 初始化ComponentTypeUUID管理系统
        global_runtime_context->component_type_uuid_system.initialize();
        if (!checkRuntimeSystem(global_runtime_context->component_type_uuid_system.getInstance())) {
            return false;
        }

        // 初始化GameObjectUUID分配器
        global_runtime_context->game_object_uuid_allocator.initialize();
        if (!checkRuntimeSystem(global_runtime_context->game_object_uuid_allocator.getInstance())) {
            return false;
        }

        // 初始化场景管理器
        global_runtime_context->scene_manager.initialize();
        UserInterface::scene_manager.ptr = global_runtime_context->scene_manager.getInstance();
        if (!checkRuntimeSystem(UserInterface::scene_manager.ptr)) {
            return false;
        }

        return true;
    }

    void MatchEngine::destroy() {
        // 销毁场景管理器
        UserInterface::scene_manager.ptr = nullptr;
        global_runtime_context->scene_manager.destory();

        // 销毁GameObjectUUID分配器
        global_runtime_context->game_object_uuid_allocator.destory();

        // 销毁ComponentTypeUUID管理系统
        global_runtime_context->component_type_uuid_system.destory();

        // 销毁事件系统
        UserInterface::event_system.ptr = nullptr;
        global_runtime_context->event_system.destory();

        // 销毁输入系统
        UserInterface::input.ptr = nullptr;
        global_runtime_context->input_system.destory();

        // 销毁窗口系统
        global_runtime_context->window_system.destory();

        // 销毁日志系统
        UserInterface::logger_system.ptr = nullptr;
        global_runtime_context->logger_system.destory();
        client_logger = nullptr;
        core_logger = nullptr;
        
        // 销毁反射系统
        UserInterface::reflect.ptr = nullptr;
        global_runtime_context->reflect_system.destory();
        
        delete global_runtime_context;
        global_runtime_context = nullptr;
    }
    
    void MatchEngine::run() {
        global_runtime_context->scene_manager->start();
        std::thread fixed_tick_thread([&]() {
            while (global_runtime_context->window_system->isAlive()) {
                global_runtime_context->scene_manager->fixedTick();
            }
        });

        while (global_runtime_context->window_system->isAlive()) {
            global_runtime_context->window_system->pollEvents();

            global_runtime_context->scene_manager->tick(0.03);
            global_runtime_context->scene_manager->swap();
            
            global_runtime_context->input_system->swapState();
        }

        fixed_tick_thread.join();
    }

    bool MatchEngine::checkRuntimeSystem(const RuntimeSystem *system) {
        if (global_runtime_context == nullptr) {
            return false;
        }
        if ((UserInterface::logger_system.ptr != nullptr) && (global_runtime_context->logger_system->getState() != RuntimeSystem::State::eInitialized)) {
            return false;
        }
        switch (system->getState()) {
        case RuntimeSystem::State::eInitialized:
            return true;
        case RuntimeSystem::State::eExited:
            MCH_CORE_ERROR("RuntimeSystem: {} has been exited.", system->getSystemName())
            return false;
        case RuntimeSystem::State::eUninitialized:
            MCH_CORE_ERROR("RuntimeSystem: {} hasn't been initialized.", system->getSystemName())
            return false;
        case RuntimeSystem::State::eFatal:
            MCH_CORE_ERROR("RuntimeSystem: {} initialize failed.", system->getSystemName())
            return false;
        }
    }
}
