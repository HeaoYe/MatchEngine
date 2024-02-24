#pragma once

#include "IDelegate.hpp"
#include "Core/CopyMove/CopyMove.hpp"
#include "Core/Thread/ThreadSafetyMode.hpp"
#include "Core/Thread/CriticalSection.hpp"

namespace MatchEngine::Core {
    template <typename FunctionType, typename ThreadSafetyModeStruct = ThreadSafetyModeThreadSafeStruct>
    class SingleDelegate {};

    template <typename ReturnType, typename ...ArgsType, typename ThreadSafetyModeStruct>
    class SingleDelegate<ReturnType(ArgsType...), ThreadSafetyModeStruct> : public IDelegate<ReturnType, ArgsType...> {
    public:
        using BaseClass = IDelegate<ReturnType, ArgsType...>;
        using FunctionType = typename BaseClass::FunctionType;
    public:
        SingleDelegate() : function(
            [](ArgsType &&...args) {
                if constexpr (!std::is_void_v<ReturnType>) {
                    return ReturnType {};
                }
            }
        ), critical_section() {}

        SingleDelegate(FunctionType &&function) : function(Move(function)), critical_section() {}
    public:
        ReturnType broadcast(ArgsType &&...args) override {
            auto scope_lock = critical_section.getScopeLock();
            if constexpr (std::is_void_v<ReturnType>) {
                function(Forward<ArgsType>(args)...);
            } else {
                return function(Forward<ArgsType>(args)...);
            }
        }

        void bind(const FunctionType &function) override {
            auto scope_lock = critical_section.getScopeLock();
            this->function = Copy(function);
        }

        void bind(FunctionType &&function) override {
            auto scope_lock = critical_section.getScopeLock();
            this->function = Move(function);
        }
    private:
        FunctionType function;
        CriticalSection<ThreadSafetyModeThreadSafeStruct::SafetyMode> critical_section;
    };
}
