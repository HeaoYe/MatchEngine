#pragma once

#include <type_traits>

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
        TPair(T1 &&t1, T2 &&t2) : first(t1), second(t2) {}
    public:
        T1 first;
        T2 second;
    };
}
