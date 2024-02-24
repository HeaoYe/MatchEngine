#pragma once

#include <functional>

namespace MatchEngine::Core {
    /**
     * @brief 委托接口
     *
     */
    template <typename ReturnType, typename ...ArgsType>
    class IDelegate {
    public:
        using FunctionType = std::function<ReturnType(ArgsType ...)>;
    public:
        virtual void bind(const FunctionType &) = 0;
        virtual void bind(FunctionType &&) = 0;
        virtual ReturnType broadcast(ArgsType &&...args) = 0;
    };
}
