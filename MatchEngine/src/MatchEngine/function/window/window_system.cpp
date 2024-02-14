#include <MatchEngine/function/window/window_system.hpp>
#include <Match/core/loader.hpp>
#include <Match/core/window.hpp>

namespace MatchEngine {
    WindowSystem::WindowSystem() {
        initializeRuntimeSystem();

        Match::setting.engine_name = "Match Engine";
        Match::setting.window_size = { 1920, 1080 };
        Match::setting.debug_mode = true;
        // Match::setting.max_in_flight_frame = 1;
        Match::set_log_level(Match::LogLevel::eError);
        api_manager = &Match::Initialize();
        Match::set_log_level(Match::LogLevel::eInfo);
        Match::runtime_setting->set_vsync(false);

        state = RuntimeSystem::State::eInitialized;
    }

    WindowSystem::~WindowSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        Match::set_log_level(Match::LogLevel::eError);
        Match::Destroy();
        Match::set_log_level(Match::LogLevel::eInfo);
    }

    void WindowSystem::pollEvents() {
        Match::window->poll_events();
    }

    bool WindowSystem::isAlive() {
        return Match::window->is_alive();
    }
}
