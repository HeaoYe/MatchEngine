#pragma once

#include "Core/Defines.hpp"
#include "Core/CopyMove/CopyMove.hpp"

#include <initializer_list>
#include <type_traits>

#include <vector>

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
        constexpr TArray() : std::vector<_T>() {}
        constexpr TArray(const TArray &other) : std::vector<_T>(Copy(other)) {}
        constexpr TArray(TArray &&other) : std::vector<_T>(Move(other)) {}
        DefineDefaultConstexprAssignmentOperator(TArray)

        constexpr TArray(std::initializer_list<T> initializer_list) : std::vector<T>(initializer_list) {}
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

        T &at(size_type index) {
            return std::vector<_T>::at(index);
        }

        const T &at(size_type index) const {
            return std::vector<_T>::at(index);
        }
    };
}
