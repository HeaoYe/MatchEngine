#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>
#include <memory>
#include <list>

namespace MatchEngine {
    class Logger {
        NoCopyMoveConstruction(Logger)
    public:
        enum class Level {
            eTrace,
            eDebug,
            eInfo,
            eWarn,
            eError,
            eFatal,
        };
    public:
        Logger(const std::string &name, Level level);
        ~Logger();

        template<typename... Args>
        void trace(Args &&... args) {
            spd_logger->trace(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void debug(Args &&... args) {
            spd_logger->debug(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void info(Args &&... args) {
            spd_logger->info(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void warn(Args &&... args) {
            spd_logger->warn(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void error(Args &&... args) {
            spd_logger->error(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void fatal(Args &&... args) {
            spd_logger->critical(std::forward<Args>(args)...);
        }
    private:
        std::shared_ptr<spdlog::logger> spd_logger;
    };

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

    extern Logger *core_logger;
    extern Logger *client_logger;
}

#define MCH_CORE_TRACE(...) ::MatchEngine::core_logger->trace(__VA_ARGS__);
#define MCH_CORE_DEBUG(...) ::MatchEngine::core_logger->debug(__VA_ARGS__);
#define MCH_CORE_INFO(...) ::MatchEngine::core_logger->info(__VA_ARGS__);
#define MCH_CORE_WARN(...) ::MatchEngine::core_logger->warn(__VA_ARGS__);
#define MCH_CORE_ERROR(...) ::MatchEngine::core_logger->error(__VA_ARGS__);
#define MCH_CORE_FATAL(...) ::MatchEngine::core_logger->fatal(__VA_ARGS__);

#define MCH_CLIENT_TRACE(...) ::MatchEngine::client_logger->trace(__VA_ARGS__);
#define MCH_CLIENT_DEBUG(...) ::MatchEngine::client_logger->debug(__VA_ARGS__);
#define MCH_CLIENT_INFO(...) ::MatchEngine::client_logger->info(__VA_ARGS__);
#define MCH_CLIENT_WARN(...) ::MatchEngine::client_logger->warn(__VA_ARGS__);
#define MCH_CLIENT_ERROR(...) ::MatchEngine::client_logger->error(__VA_ARGS__);
#define MCH_CLIENT_FATAL(...) ::MatchEngine::client_logger->fatal(__VA_ARGS__);
