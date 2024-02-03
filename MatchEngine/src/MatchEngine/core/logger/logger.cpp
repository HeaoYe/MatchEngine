#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine {
    Logger *core_logger = nullptr;
    Logger *client_logger = nullptr;

    static spdlog::level::level_enum convert(Logger::Level level) {
        switch (level) {
        case Logger::Level::eTrace:
            return spdlog::level::level_enum::trace;
        case Logger::Level::eDebug:
            return spdlog::level::level_enum::debug;
        case Logger::Level::eInfo:
            return spdlog::level::level_enum::info;
        case Logger::Level::eWarn:
            return spdlog::level::level_enum::warn;
        case Logger::Level::eError:
            return spdlog::level::level_enum::err;
        case Logger::Level::eFatal:
            return spdlog::level::level_enum::critical;
        }
    }

    Logger::Logger(const std::string &name, Level level) {
        spd_logger = spdlog::stdout_color_mt(name);
        spd_logger->set_level(convert(level));
    }

    Logger::~Logger() {
        spd_logger.reset();
    }
}
