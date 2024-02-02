#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <string>

namespace MatchEngine {

    // 运行时系统
    class RuntimeSystem {
        NoCopyMoveConstruction(RuntimeSystem)
    public:
        enum class State {
            eUninitialized,
            eInitialized,
            eExited,
            eFatal,
        };
    public:
        RuntimeSystem();
        virtual ~RuntimeSystem();
        virtual std::string getSystemName() const { return "UnknownRuntimeSystem"; }
        virtual State getState() const { return state; }
    protected:
        State state { RuntimeSystem::State::eUninitialized };
    };
}
