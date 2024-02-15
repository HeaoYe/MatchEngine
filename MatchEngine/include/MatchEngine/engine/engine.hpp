#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>

namespace MatchEngine {
    class MatchEngine {
        DefaultNoCopyMoveConstruction(MatchEngine)
    public:
        bool initialize();
        void destroy();
        void gameLoop();
    private:
        bool checkRuntimeSystem(const RuntimeSystem *system);
    };
}
