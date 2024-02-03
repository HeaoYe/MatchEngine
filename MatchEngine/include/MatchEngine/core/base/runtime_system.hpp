#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <string>

namespace MatchEngine {

    // 运行时系统
    class RuntimeSystem {
        DefaultNoCopyMoveConstruction(RuntimeSystem)
    public:
        enum class State {
            eUninitialized,
            eInitialized,
            eExited,
            eFatal,
        };
    public:
        virtual ~RuntimeSystem() = default;
        virtual std::string getSystemName() const { return "UnknownRuntimeSystem"; }
        virtual State getState() const { return state; }
    protected:
        void initializeRuntimeSystem();
        void destoryRuntimeSystem();
    protected:
        State state { RuntimeSystem::State::eUninitialized };
    };
}
