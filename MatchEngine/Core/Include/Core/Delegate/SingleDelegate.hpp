#pragma once

#include "IDelegate.hpp"
#include "Core/Misc/Copy.hpp"
#include "Core/Misc/Move.hpp"
#include "Core/Misc/Forward.hpp"
#include "Core/Misc/HandleAllocator.hpp"
#include "Core/Container/Pair.hpp"
#include "Core/Thread/ThreadSafetyMode.hpp"
#include "Core/Thread/CriticalSection.hpp"

namespace MatchEngine::Core {
    template <typename FunctionType, typename ThreadSafetyModeStruct = ThreadSafetyModeThreadSafeStruct>
    class TSingleDelegate;

    /**
     * @brief 单播委托
     *
     * @tparam ReturnType 委托函数的返回值类型
     * @tparam ArgsType 委托函数的参数类型
     * @tparam ThreadSafetyModeStruct 委托的线程安全模式
     */
    template <typename ReturnType, typename ...ArgsType, typename ThreadSafetyModeStruct>
    class TSingleDelegate<ReturnType(ArgsType...), ThreadSafetyModeStruct> : public IDelegate<ReturnType(ArgsType...)> {
        template <typename, typename>
        friend class TSingleDelegate;

        using Super = IDelegate<ReturnType(ArgsType...)>;
        using FunctionType = typename Super::FunctionType;
        using DelegateHandleType = typename Super::DelegateHandleType;
        inline static const FunctionType EmptyDelegateFunction = [](ArgsType &&...args) { if constexpr (!std::is_void_v<ReturnType>) { return ReturnType {}; } };
    public:
        TSingleDelegate() : handle_allocator(), critical_section() {
            this->function = { InvalidHandleTrait<DelegateHandleType>::value, Copy(EmptyDelegateFunction) };
        }

        TSingleDelegate(const TSingleDelegate &other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Copy(other.function);
            this->handle_allocator = Copy(other.handle_allocator);
        }

        TSingleDelegate(TSingleDelegate &&other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Move(other.function);
            other.function = { InvalidHandleTrait<DelegateHandleType>::value, Copy(EmptyDelegateFunction) };
            this->handle_allocator = Move(other.handle_allocator);
        }

        TSingleDelegate &operator=(const TSingleDelegate &other) {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Copy(other.function);
            this->handle_allocator = Copy(other.handle_allocator);
            return *this;
        }

        TSingleDelegate &operator=(TSingleDelegate &&other) {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Move(other.function);
            other.function = { InvalidHandleTrait<DelegateHandleType>::value, Copy(EmptyDelegateFunction) };
            this->handle_allocator = Move(other.handle_allocator);
            return *this;
        }

        template <typename _ThreadSafetyModeStruct>
        TSingleDelegate(const TSingleDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Copy(other.function);
            this->handle_allocator = Copy(other.handle_allocator);
        }

        template <typename _ThreadSafetyModeStruct>
        TSingleDelegate(TSingleDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &&other) : critical_section() {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Move(other.function);
            other.function = { InvalidHandleTrait<DelegateHandleType>::value, Copy(EmptyDelegateFunction) };
            this->handle_allocator = Move(other.handle_allocator);
        }

        template <typename _ThreadSafetyModeStruct>
        TSingleDelegate &operator=(const TSingleDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &other) {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Copy(other.function);
            this->handle_allocator = Copy(other.handle_allocator);
            return *this;
        }

        template <typename _ThreadSafetyModeStruct>
        TSingleDelegate &operator=(TSingleDelegate<ReturnType(ArgsType...), _ThreadSafetyModeStruct> &&other) {
            auto other_scope_lock = other.critical_section.getScopeLock();
            auto this_scope_lock = this->critical_section.getScopeLock();
            this->function = Move(other.function);
            other.function = { InvalidHandleTrait<DelegateHandleType>::value, Copy(EmptyDelegateFunction) };
            this->handle_allocator = Move(other.handle_allocator);
            return *this;
        }

        TSingleDelegate(const FunctionType &function) : function(Copy(function)), handle_allocator(), critical_section() {}

        TSingleDelegate(FunctionType &&function) : function(Move(function)), handle_allocator(), critical_section() {}
    public:
        DelegateHandleType bind(const FunctionType &function) override {
            auto scope_lock = this->critical_section.getScopeLock();
            this->function = { handle_allocator.allocate(), Copy(function) };
            return this->function.first;
        }

        DelegateHandleType bind(FunctionType &&function) override {
            auto scope_lock = this->critical_section.getScopeLock();
            this->function = { handle_allocator.allocate(), Move(function) };
            return this->function.first;
        }

        void removeDelegateFunction(DelegateHandleType handle) override {
            auto scope_lock = this->critical_section.getScopeLock();
            if (this->function.first == handle) {
                this->function = { InvalidHandleTrait<DelegateHandleType>::value, Copy(EmptyDelegateFunction) };
            }
        }

        ReturnType broadcast(ArgsType &&...args) override {
            auto scope_lock = this->critical_section.getScopeLock();
            if constexpr (std::is_void_v<ReturnType>) {
                this->function.second(Forward<ArgsType>(args)...);
            } else {
                return this->function.second(Forward<ArgsType>(args)...);
            }
        }

        bool hasDelegateFunction(DelegateHandleType handle) override {
            auto scope_lock = this->critical_section.getScopeLock();
            return this->function.first == handle;
        }
    private:
        TPair<DelegateHandleType, FunctionType> function;
        // 因为allocate操作是在critical_section中执行, 所以不需要线程安全的句柄分配器
        THandleAllocator<DelegateHandleType, EThreadSafetyMode::eNotThreadSafe> handle_allocator;
        CriticalSection<ThreadSafetyModeThreadSafeStruct::SafetyMode> critical_section;
    };
}
