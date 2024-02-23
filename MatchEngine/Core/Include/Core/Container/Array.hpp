#pragma once

#include "../CopyMove/CopyMove.hpp"
#include <vector>
#include <initializer_list>
#include <type_traits>

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
        using T = std::decay_t<_T>;
        using size_type = size_t;
    public:
        TArray() : std::vector<_T>() {}
        TArray(const TArray &other) : std::vector<_T>(Copy(other)) {}
        TArray(TArray &&other) : std::vector<_T>(Move(other)) {}

        TArray(const std::initializer_list<T> &initializer_list) : std::vector<T>(initializer_list) {}
    public:
        void add(const T &element) {
            std::vector<_T>::push_back(element);
        }

        void add(T &&element) {
            std::vector<_T>::push_back(element);
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
