#pragma once

#include "Core/Defines.hpp"
#include "Core/Misc/Copy.hpp"
#include "Core/Misc/Move.hpp"

#include <type_traits>

#include <utility>

namespace MatchEngine::Core {
    /**
     * @brief 一对数据, 相当于std::pair
     *
     * @tparam _T1 第一个数据的类型
     * @tparam _T2 第二个数据的类型
     *
     * TODO: 进行内存管理
     */
    template <typename _T1, typename _T2>
    class TPair {
        using T1 = std::decay_t<_T1>;
        using T2 = std::decay_t<_T2>;
    public:
        constexpr TPair() : first(), second() {}
        constexpr TPair(const TPair &other) : first(Copy(other.first)), second(Copy(other.second)) {}
        constexpr TPair(TPair &&other) : first(Move(other.first)), second(Move(other.second)) {}
        DefineDefaultConstexprAssignmentOperator(TPair)

        constexpr TPair(const T1 &t1, const T2 &t2) : first(Copy(t1)), second(Copy(t2)) {}
        constexpr TPair(const T1 &t1, T2 &&t2) : first(Copy(t1)), second(Move(t2)) {}
        constexpr TPair(T1 &&t1, const T2 &t2) : first(Move(t1)), second(Copy(t2)) {}
        constexpr TPair(T1 &&t1, T2 &&t2) : first(Move(t1)), second(Move(t2)) {}
    public:
        T1 first;
        T2 second;
    public:
        /**
         * @brief 仅为了兼容std::pair, 请不要使用这个构造函数
         *
         */
        constexpr TPair(const std::pair<T1, T2> &other) : first(Copy(other.first)), second(Copy(other.second)) {}

        /**
         * @brief 仅为了兼容std::pair, 请不要使用这个构造函数
         *
         */
        constexpr TPair(std::pair<T1, T2> &&other) : first(Move(other.first)), second(Move(other.second)) {}
    };
}
