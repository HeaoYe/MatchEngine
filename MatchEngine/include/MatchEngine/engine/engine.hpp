#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>

namespace MatchEngine {
    class MatchEngine {
        DefaultNoCopyMoveConstruction(MatchEngine)
    public:
        bool initialize();
        void destroy();
        void run();
    private:
        bool checkRuntimeSystem(const RuntimeSystem *system);
    };
}
