#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>

namespace MatchEngine {
    class MatchEngine {
        DefaultNoCopyMoveConstruction(MatchEngine)
    public:
        bool initialize();
        void destroy();
        void run();
        void loadScene(const std::string &name);
    private:
        bool checkRuntimeSystem(const RuntimeSystem *system);
    };
}
