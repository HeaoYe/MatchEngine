#pragma once

#include <set>
#include <initializer_list>
#include <type_traits>

namespace MatchEngine::Core {
    /**
     * @brief 元组, 不可有重复元素的数组
     *
     * @tparam _T 元组的数据类型
     * @tparam Compare 元组的数据的比较方式, 默认为std::less
     *
     * TODO: 不依赖std::set进行实现
     * TODO: 进行内存管理
     */
    template <typename _T, typename Compare = std::less<_T>>
    class TSet : std::set<_T> {
        using T = std::decay_t<_T>;
        using size_type = size_t;
    public:
        TSet() : std::set<_T, Compare>() {}
        TSet(const TSet &other) : std::set<_T, Compare>(Copy(other)) {}
        TSet(TSet &&other) : std::set<_T, Compare>(Move(other)) {}

        TSet(const std::initializer_list<T> &initializer_list) : std::set<_T, Compare>(initializer_list) {}
    public:
        void add(const T &element) {
            std::set<_T, Compare>::insert(element);
        }

        void add(T &&element) {
            std::set<_T, Compare>::insert(element);
        }

        void remove(const T &element) {
            std::set<_T, Compare>::erase(element);
        }

        void remove(T &&element) {
            std::set<_T, Compare>::erase(element);
        }

        size_type size() const {
            return std::set<_T, Compare>::size();
        }

        void clear() {
            std::set<_T, Compare>::clear();
        }

        auto begin() {
            return std::set<_T, Compare>::begin();
        }

        auto end() {
            return std::set<_T, Compare>::end();
        }

        auto &at(size_type index) {
            return std::set<_T, Compare>::at(index);
        }

        const auto &at(size_type index) const {
            return std::set<_T, Compare>::at(index);
        }
    };
}
