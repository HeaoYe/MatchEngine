#pragma once

#include <functional>
#include <cstdint>

namespace MatchEngine::Core {
    template <typename FunctionType>
    class IDelegate;

    /**
     * @brief 委托接口
     *
     */
    template <typename ReturnType, typename ...ArgsType>
    class IDelegate<ReturnType(ArgsType...)> {
    public:
        using FunctionType = std::function<ReturnType(ArgsType ...)>;
        using DelegateHandleType = uint64_t;
    public:
        virtual DelegateHandleType bind(const FunctionType &) = 0;
        virtual DelegateHandleType bind(FunctionType &&) = 0;

        virtual void removeDelegateFunction(DelegateHandleType handle) = 0;

        virtual ReturnType broadcast(ArgsType &&...args) = 0;

        virtual bool hasDelegateFunction(DelegateHandleType handle) = 0;
    };
}
