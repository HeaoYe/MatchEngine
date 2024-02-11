#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include <list>

namespace MatchEngine {
    class LoggerSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(LoggerSystem, Logger::Level default_level)
    public:
        Logger *createLogger(const std::string &name);
    private:
        Logger::Level default_level;
        std::list<std::unique_ptr<Logger>> loggers;
    };
}
