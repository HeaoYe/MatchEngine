#pragma once

#include "Core/Defines.hpp"
#include "Core/Misc/Copy.hpp"
#include "Core/Misc/Move.hpp"
#include "Pair.hpp"

#include <type_traits>

#include <map>

namespace MatchEngine::Core {
    /**
     * @brief 键值对容器, 相当于std::map
     *
     * @tparam _Key 键的数据类型
     * @tparam _Value 值的数据类型
     * @tparam Compare 键的数据的比较方式, 默认为std::less
     *
     * TODO: 不依赖std::map进行实现
     * TODO: 进行内存管理
     */
    template <typename _Key, typename _Value, typename Compare = std::less<_Key>>
    class TMap : std::map<_Key, _Value> {
        using Key = std::decay_t<_Key>;
        using Value = std::decay_t<_Value>;
        using KeyValuePair = TPair<const Key, Value>;
        using size_type = size_t;
    public:
        constexpr TMap() : std::map<_Key, _Value, Compare>() {}
        constexpr TMap(const TMap &other) : std::map<_Key, _Value, Compare>(Copy(other)) {}
        constexpr TMap(TMap &&other) : std::map<_Key, _Value, Compare>(Move(other)) {}
        DefineDefaultConstexprAssignmentOperator(TMap)

        constexpr TMap(std::initializer_list<std::pair<const Key, Value>> initializer_list) : std::map<_Key, _Value, Compare>(initializer_list) {}
    public:
        void add(const Key &key, const Value &value) {
            std::map<_Key, _Value, Compare>::insert(key, value);
        }

        void remove(const Key &key) {
            std::map<_Key, _Value, Compare>::erase(key);
        }

        size_type size() const {
            return std::map<_Key, _Value, Compare>::size();
        }

        void clear() {
            std::map<_Key, _Value, Compare>::clear();
        }

        auto begin() {
            return std::map<_Key, _Value, Compare>::begin();
        }

        auto end() {
            return std::map<_Key, _Value, Compare>::end();
        }

        Value &at(const Key &key) {
            return std::map<_Key, _Value, Compare>::at(key);
        }

        const Value &at(const Key &key) const {
            return std::map<_Key, _Value, Compare>::at(key);
        }

        KeyValuePair find(const Key &key) const {
            return KeyValuePair(*std::map<_Key, _Value, Compare>::find(key));
        }
    };
}
