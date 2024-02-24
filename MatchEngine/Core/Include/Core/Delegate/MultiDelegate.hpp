#pragma once

#include "IDelegate.hpp"
#include "Core/Misc/Copy.hpp"
#include "Core/Misc/Move.hpp"
#include "Core/Misc/Forward.hpp"
#include "Core/Misc/HandleAllocator.hpp"
#include "Core/Container/Map.hpp"
#include "Core/Thread/ThreadSafetyMode.hpp"
#include "Core/Thread/CriticalSection.hpp"

namespace MatchEngine::Core {
    template <typename FunctionType, typename ThreadSafetyModeStruct = ThreadSafetyModeThreadSafeStruct>
    class TMultiDelegate {};

    /**
     * @brief 多播委托
     *
     * @tparam ReturnType 委托函数的返回值类型
     * @tparam ArgsType 委托函数的参数类型
     * @tparam ThreadSafetyModeStruct 委托的线程安全模式
     */
    template <typename ReturnType, typename ...ArgsType, typename ThreadSafetyModeStruct>
    class TMultiDelegate<ReturnType(ArgsType...), ThreadSafetyModeStruct> : public IDelegate<ReturnType, ArgsType...> {
        template <typename, typename>
        friend class TSingleDelegate;

        using BaseClass = IDelegate<ReturnType, ArgsType...>;
        using FunctionType = typename BaseClass::FunctionType;
        using DelegateHandleType = typename BaseClass::DelegateHandleType;
    public:
        TMultiDelegate() : functions(), handle_allocator(), critical_section() {}

        TMultiDelegate(const TMultiDelegate &other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->functions = Copy(other.functions);
            this->handle_allocator = Copy(other.handle_allocator);
        }

        TMultiDelegate(TMultiDelegate &&other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->functions = Move(other.functions);
            this->handle_allocator = Move(other.handle_allocator);
        }

        DefineDefaultAssignmentOperator(TMultiDelegate)

        template <typename _ThreadSafetyModeStruct>
        TMultiDelegate(const TMultiDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->functions = Copy(other.functions);
            this->handle_allocator = Copy(other.handle_allocator);
        }

        template <typename _ThreadSafetyModeStruct>
        TMultiDelegate(TMultiDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &&other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->functions = Move(other.functions);
            this->handle_allocator = Move(other.handle_allocator);
        }

        template <typename _ThreadSafetyModeStruct>
        TMultiDelegate &operator=(const TMultiDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &other) {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->functions = Copy(other.functions);
            this->handle_allocator = Copy(other.handle_allocator);
            return *this;
        }

        template <typename _ThreadSafetyModeStruct>
        TMultiDelegate &operator=(TMultiDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &&other) {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->functions = Move(other.functions);
            this->handle_allocator = Move(other.handle_allocator);
            return *this;
        }
    public:
        DelegateHandleType bind(const FunctionType &function) override {
            auto scope_lock = this->critical_section.getScopeLock();
            auto handle = handle_allocator.allocate();
            this->functions.add(handle, Copy(function));
            return handle;
        }

        DelegateHandleType bind(FunctionType &&function) override {
            auto scope_lock = this->critical_section.getScopeLock();
            auto handle = handle_allocator.allocate();
            this->functions.add(handle, Move(function));
            return handle;
        }

        void removeDelegateFunction(DelegateHandleType handle) override {
            auto scope_lock = this->critical_section.getScopeLock();
            this->functions.remove(handle);
        }

        ReturnType broadcast(ArgsType &&...args) override {
            auto scope_lock = this->critical_section.getScopeLock();
            if constexpr (std::is_void_v<ReturnType>) {
                for (auto &[handle, function] : this->functions) {
                    function(Forward<ArgsType>(args)...);
                }
            } else {
                ReturnType result;
                for (auto &[handle, function] : this->functions) {
                    result = function(Forward<ArgsType>(args)...);
                }
                return result;
            }
        }

        bool hasDelegateFunction(DelegateHandleType handle) override {
            auto scope_lock = this->critical_section.getScopeLock();
            return this->functions.has(handle);
        }
    private:
        TMap<DelegateHandleType, FunctionType> functions;
        THandleAllocator<DelegateHandleType, ThreadSafetyModeThreadSafeStruct::SafetyMode> handle_allocator;
        CriticalSection<ThreadSafetyModeThreadSafeStruct::SafetyMode> critical_section;
    };
}
