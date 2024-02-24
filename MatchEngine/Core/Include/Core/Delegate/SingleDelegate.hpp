#pragma once

#include "IDelegate.hpp"
#include "Core/Misc/Copy.hpp"
#include "Core/Misc/Move.hpp"
#include "Core/Misc/Forward.hpp"
#include "Core/Thread/ThreadSafetyMode.hpp"
#include "Core/Thread/CriticalSection.hpp"

namespace MatchEngine::Core {
    template <typename FunctionType, typename ThreadSafetyModeStruct = ThreadSafetyModeThreadSafeStruct>
    class SingleDelegate {};

    /**
     * @brief 单播委托
     *
     * @tparam ReturnType 委托函数的返回值类型
     * @tparam ArgsType 委托函数的参数类型
     * @tparam ThreadSafetyModeStruct 委托的线程安全模式
     */
    template <typename ReturnType, typename ...ArgsType, typename ThreadSafetyModeStruct>
    class SingleDelegate<ReturnType(ArgsType...), ThreadSafetyModeStruct> : public IDelegate<ReturnType, ArgsType...> {
        template <typename, typename>
        friend class SingleDelegate;

        using BaseClass = IDelegate<ReturnType, ArgsType...>;
        using FunctionType = typename BaseClass::FunctionType;
        inline static const FunctionType EmptyDelegateFunction = [](ArgsType &&...args) { if constexpr (!std::is_void_v<ReturnType>) { return ReturnType {}; } };
    public:
        SingleDelegate() : function(Copy(EmptyDelegateFunction)), critical_section() {}

        template <typename _ThreadSafetyModeStruct>
        SingleDelegate(const SingleDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Copy(other.function);
        }

        template <typename _ThreadSafetyModeStruct>
        SingleDelegate(SingleDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &&other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Move(other.function);
            other.function = Copy(EmptyDelegateFunction);
        }

        template <typename _ThreadSafetyModeStruct>
        SingleDelegate &operator=(const SingleDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &other) {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Copy(other.function);
            return *this;
        }

        template <typename _ThreadSafetyModeStruct>
        SingleDelegate &operator=(SingleDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &&other) {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Move(other.function);
            other.function = Copy(EmptyDelegateFunction);
            return *this;
        }

        SingleDelegate(const FunctionType &function) : function(Copy(function)), critical_section() {}

        SingleDelegate(FunctionType &&function) : function(Move(function)), critical_section() {}
    public:
        void bind(const FunctionType &function) override {
            auto scope_lock = this->critical_section.getScopeLock();
            this->function = Copy(function);
        }

        void bind(FunctionType &&function) override {
            auto scope_lock = this->critical_section.getScopeLock();
            this->function = Move(function);
        }

        ReturnType broadcast(ArgsType &&...args) override {
            auto scope_lock = this->critical_section.getScopeLock();
            if constexpr (std::is_void_v<ReturnType>) {
                this->function(Forward<ArgsType>(args)...);
            } else {
                return this->function(Forward<ArgsType>(args)...);
            }
        }
    private:
        FunctionType function;
        CriticalSection<ThreadSafetyModeThreadSafeStruct::SafetyMode> critical_section;
    };
}
