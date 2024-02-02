#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine {
    RuntimeSystem::RuntimeSystem() {
        if (core_logger != nullptr) {
            MCH_CORE_WARN("Initialize RuntimeSystem: {}", getSystemName())
        }
    }

    RuntimeSystem::~RuntimeSystem() {
        if (core_logger != nullptr) {
            MCH_CORE_WARN("Destory RuntimeSystem: {}", getSystemName())
        }
    }
}
