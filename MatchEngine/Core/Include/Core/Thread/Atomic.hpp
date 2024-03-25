#pragma once

#include <type_traits>
#include <atomic>

namespace MatchEngine::Core {
    /**
     * @brief 原子变量
     *
     * @tparam T 整数类型
     */
    template <typename T>
    class TAtomic {
        static_assert(std::is_integral_v<T>, "");
    public:
        TAtomic() = default;
        TAtomic(T value) : data(value) {}
        T fetchIncrease() { return std::atomic_fetch_add(&data, 1); }
        T fetchDecrease() { return std::atomic_fetch_sub(&data, 1); }
        T fetchAdd(T value) { return std::atomic_fetch_add(&data, value); }
        T fetchSub(T value) { return std::atomic_fetch_sub(&data, value); }
        T load() const { return data.load(); }
        void store(T value) { data.store(value); }
        T exchange(T value) { return data.exchange(value); }
        bool compareExchange(T &old_value, T value) { return data.compare_exchange_strong(old_value, value); }
    private:
        std::atomic<T> data {};
    };
}
