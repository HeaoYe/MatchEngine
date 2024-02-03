#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine {
    void RuntimeSystem::initializeRuntimeSystem() {
        if (core_logger != nullptr) {
            MCH_CORE_WARN("Initialize RuntimeSystem: {}", getSystemName())
        }
    }

    void RuntimeSystem::destoryRuntimeSystem() {
        if (core_logger != nullptr) {
            MCH_CORE_WARN("Destory RuntimeSystem: {}", getSystemName())
        }
    }
}
