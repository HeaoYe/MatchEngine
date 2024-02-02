#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/engine/runtime_context.hpp>

namespace MatchEngine {
    class MatchEngine {
        DefaultNoCopyMoveConstruction(MatchEngine)
    public:
        void initialize();
        void destroy();
        void run();
    private:
        bool check_runtime_system(const RuntimeSystem *system);
    };
}
