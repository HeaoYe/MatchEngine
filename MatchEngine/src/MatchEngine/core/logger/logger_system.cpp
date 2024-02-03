#include <MatchEngine/core/logger/logger_system.hpp>

namespace MatchEngine {
    LoggerSystem::LoggerSystem(Logger::Level default_level) : default_level(default_level) {
        core_logger = createLogger("MatchEngine Core");
        client_logger = createLogger("MatchEngine Client");

        initializeRuntimeSystem();
        state = RuntimeSystem::State::eInitialized;
    }

    Logger *LoggerSystem::createLogger(const std::string &name) {
        loggers.push_front(std::make_unique<Logger>(name, default_level));
        return loggers.front().get();
    };

    LoggerSystem::~LoggerSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        for (auto &logger : loggers) {
            logger.reset();
        }
        loggers.clear();
        spdlog::shutdown();
    }
}
