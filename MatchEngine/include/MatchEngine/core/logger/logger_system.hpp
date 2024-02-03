#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include <list>

namespace MatchEngine {
    class LoggerSystem final : public RuntimeSystem {
        NoCopyMoveConstruction(LoggerSystem)
    public:
        LoggerSystem(Logger::Level default_level);
        ~LoggerSystem() override;

        Logger *createLogger(const std::string &name);

        std::string getSystemName() const override { return "LoggerSystem"; }
    private:
        Logger::Level default_level;
        std::list<std::unique_ptr<Logger>> loggers;
    };
}
