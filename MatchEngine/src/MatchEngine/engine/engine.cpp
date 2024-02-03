#include <MatchEngine/engine/engine.hpp>
#include <MatchEngine/function/user_interface.hpp>
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

        // 初始化日志系统
        global_runtime_context->logger_system.initialize(Logger::Level::eInfo);
        logger_system.ptr = global_runtime_context->logger_system.getInstance();
        if (!checkRuntimeSystem(logger_system.ptr)) {
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
        input.ptr = global_runtime_context->input_system.getInstance();
        if (!checkRuntimeSystem(input.ptr)) {
            return false;
        }

        // 初始化事件系统
        global_runtime_context->event_system.initialize();
        event_system.ptr = global_runtime_context->event_system.getInstance();
        if (!checkRuntimeSystem(event_system.ptr)) {
            return false;
        }

        return true;
    }

    void MatchEngine::destroy() {
        // 销毁事件系统
        global_runtime_context->event_system.destory();

        // 销毁输入系统
        global_runtime_context->input_system.destory();

        // 销毁窗口系统
        global_runtime_context->window_system.destory();

        // 销毁日志系统
        global_runtime_context->logger_system.destory();
        client_logger = nullptr;
        core_logger = nullptr;
        
        delete global_runtime_context;
        global_runtime_context = nullptr;
    }
    
    void MatchEngine::run() {
        while (global_runtime_context->window_system->isAlive()) {
            global_runtime_context->window_system->pollEvents();
            global_runtime_context->input_system->swapState();
        }
    }

    bool MatchEngine::checkRuntimeSystem(const RuntimeSystem *system) {
        if (global_runtime_context == nullptr) {
            return false;
        }
        if (global_runtime_context->logger_system->getState() != RuntimeSystem::State::eInitialized) {
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
