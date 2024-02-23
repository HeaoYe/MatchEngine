#pragma once

#include <vector>
#include <initializer_list>

namespace MatchEngine::Core {
    /**
     * @brief 数组, 相当于std::vector
     *
     * @tparam _T 数组的数据类型
     *
     * TODO: 不依赖std::vector进行实现
     * TODO: 进行内存管理
     */
    template <typename _T>
    class TArray : std::vector<_T> {
        using size_type = size_t;
        using T = std::decay_t<_T>;
    public:
        TArray(const std::initializer_list<T> &initializer_list) : std::vector<T>(initializer_list) {}
    public:
        void add(const T &t) {
            std::vector<_T>::push_back(t);
        }

        void add(T &&t) {
            std::vector<_T>::push_back(t);
        }

        void remove(size_type index) {
            std::vector<_T>::erase(begin() + index);
        }

        size_type size() const {
            return std::vector<_T>::size();
        }

        void clear() {
            std::vector<_T>::clear();
        }

        auto begin() {
            return std::vector<_T>::begin();
        }

        auto end() {
            return std::vector<_T>::end();
        }

        auto &at(size_type index) {
            return std::vector<_T>::at(index);
        }

        const auto &at(size_type index) const {
            return std::vector<_T>::at(index);
        }
    };
}
