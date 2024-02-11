#pragma once

#include <MatchEngine/core/base/singleton.hpp>
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

    #define DECLARE_RUNTIME_SYSTEM(cls_name, ...) \
        NoCopyMoveConstruction(cls_name) \
        friend Singleton<cls_name>; \
    private: \
        cls_name(__VA_ARGS__); \
        ~cls_name() override; \
    public: \
        std::string getSystemName() const override { return #cls_name; } \
    private:
}
