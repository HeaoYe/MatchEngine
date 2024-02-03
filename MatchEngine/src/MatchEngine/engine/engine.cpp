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
        // 创建日志
        core_logger = global_runtime_context->logger_system->createLogger("MatchEngine Core");
        client_logger = global_runtime_context->logger_system->createLogger("MatchEngine Client");

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

        // 销毁日志系统
        client_logger = nullptr;
        core_logger = nullptr;
        global_runtime_context->logger_system.destory();
        
        delete global_runtime_context;
        global_runtime_context = nullptr;
    }
    
    void MatchEngine::run() {
        MCH_CORE_INFO("running...")
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
