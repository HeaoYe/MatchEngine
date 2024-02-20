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
        global_runtime_context->editor_mode = editor_mode;

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

        // 初始化事件系统
        global_runtime_context->event_system.initialize();
        UserInterface::event_system.ptr = global_runtime_context->event_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::event_system.ptr)) {
            return false;
        }

        // 初始化输入系统
        global_runtime_context->input_system.initialize();
        UserInterface::input.ptr = global_runtime_context->input_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::input.ptr)) {
            return false;
        }

        // 初始化定时器系统
        global_runtime_context->timer_system.initialize();
        UserInterface::timer_system.ptr = global_runtime_context->timer_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::timer_system.ptr)) {
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

        // 初始化渲染系统
        global_runtime_context->render_system.initialize();
        if (!checkRuntimeSystem(global_runtime_context->render_system.getInstance())) {
            return false;
        }

        // 初始化相机系统
        global_runtime_context->camera_system.initialize();
        UserInterface::camera_system.ptr = global_runtime_context->camera_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::camera_system.ptr)) {
            return false;
        }

        // 初始化资产系统
        global_runtime_context->assets_system.initialize();
        UserInterface::assets_system.ptr = global_runtime_context->assets_system.getInstance();
        if (!checkRuntimeSystem(UserInterface::assets_system.ptr)) {
            return false;
        }

        // 初始化场景管理器
        global_runtime_context->scene_manager.initialize();
        UserInterface::scene_manager.ptr = global_runtime_context->scene_manager.getInstance();
        if (!checkRuntimeSystem(UserInterface::scene_manager.ptr)) {
            return false;
        }

        global_runtime_context->render_system->createRenderResource();
        if (editor_mode) {
            hook_imgui_layer();
        }
        global_runtime_context->render_system->getRenderer()->start();

        dt = 0.03;
        main_loop_timer = global_runtime_context->timer_system->createTimer();
        fixed_tick_timer = global_runtime_context->timer_system->createTimer();
        benchmark = global_runtime_context->timer_system->createTimer();
        stop();
        
        // 在一个单独的线程，进行30FPS的固定频率tick
        fixed_tick_thread = std::make_unique<std::thread>([this]() {
            while (global_runtime_context->window_system->isAlive()) {
                if ((!fixed_tick_timer->is_suspended()) && (!main_loop_timer->is_suspended())) {
                    global_runtime_context->scene_manager->fixedTick();
                }
                fixed_tick_timer->tick(std::chrono::milliseconds(static_cast<int>(Game::fixed_tick_delta_time * 1000)));
            }
        });

        return true;
    }

    void MatchEngine::destroy() {
        global_runtime_context->render_system->waitRenderDevice();

        // 销毁场景管理器
        UserInterface::scene_manager.ptr = nullptr;
        global_runtime_context->scene_manager.destory();

        // 销毁资产系统
        UserInterface::assets_system.ptr = nullptr;
        global_runtime_context->assets_system.destory();

        // 销毁相机系统
        UserInterface::camera_system.ptr = nullptr;
        global_runtime_context->camera_system.destory();

        // 销毁渲染系统
        global_runtime_context->render_system.destory();

        // 销毁GameObjectUUID分配器
        global_runtime_context->game_object_uuid_allocator.destory();

        // 销毁ComponentTypeUUID管理系统
        global_runtime_context->component_type_uuid_system.destory();

        // 销毁定时器系统
        UserInterface::timer_system.ptr = nullptr;
        global_runtime_context->timer_system.destory();

        // 销毁输入系统
        UserInterface::input.ptr = nullptr;
        global_runtime_context->input_system.destory();

        // 销毁事件系统
        UserInterface::event_system.ptr = nullptr;
        global_runtime_context->event_system.destory();

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

    void MatchEngine::start() {
        global_runtime_context->scene_manager->start();

        main_loop_timer->resume();
        fixed_tick_timer->resume();
        main_loop_timer->tick();
        fixed_tick_timer->tick();
    }

    void MatchEngine::stop() {
        main_loop_timer->suspend();
        fixed_tick_timer->suspend();
    }

    void MatchEngine::tickLogic() {
        if (!main_loop_timer->is_suspended()) {
            global_runtime_context->scene_manager->swap();
        }

        benchmark->tick();
        if (!main_loop_timer->is_suspended()) {
            global_runtime_context->scene_manager->tick(dt);
        }
        float benchmark_dt = benchmark->tick();
        MCH_CORE_INFO("BENCHMARK: Logic Delta Time(ms): {}", benchmark_dt * 1000);
    }

    void MatchEngine::tickRender() {
        benchmark->tick();
        global_runtime_context->render_system->render();
        float benchmark_dt = benchmark->tick();
        MCH_CORE_INFO("BENCHMARK: Render CPU Delta Time(ms): {}", benchmark_dt * 1000);

        dt = main_loop_timer->tick();
    }

    void MatchEngine::tickInputState() {
        global_runtime_context->input_system->swapState();

    }
    
    void MatchEngine::waitFixedTickThread() {
        fixed_tick_thread->join();
        fixed_tick_thread.reset();
    }

    void MatchEngine::gameLoop() {
        start();
        // 不限频率的主游戏循环更新
        while (global_runtime_context->window_system->isAlive()) {
            global_runtime_context->window_system->pollEvents();
            tickLogic();
            tickRender();
            tickInputState();
        }
        waitFixedTickThread();
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
