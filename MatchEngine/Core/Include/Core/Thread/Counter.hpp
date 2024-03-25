#pragma once

#include "ThreadSafetyMode.hpp"
#include "Atomic.hpp"
#include "Sleep.hpp"

namespace MatchEngine::Core {
    template <typename T, ThreadSafetyMode>
    class TCounter;

    /**
     * @brief 线程安全计数器
     *
     * @tparam T
     */
    template <typename T>
    class TCounter<T, ThreadSafetyMode::eThreadSafe> {
        template <typename, ThreadSafetyMode>
        friend class TCounter;
    public:
        TCounter() = default;
        TCounter(T count) { this->count.store(count); }
        TCounter(const TCounter &other) { count.store(other.count.load()); }
        TCounter& operator=(const TCounter &other) { count.store(other.count.load()); return *this; }
        TCounter(TCounter &&other) { count.store(other.count.load()); other.reset(); }
        TCounter& operator=(TCounter &&other) { count.store(other.count.load()); other.reset(); return *this; }

        TCounter(const TCounter<T, ThreadSafetyMode::eNotThreadSafe> &other) { count.store(other.count); }
        TCounter& operator=(const TCounter<T, ThreadSafetyMode::eNotThreadSafe> &other) { count.store(other.count); return *this; }
        TCounter(TCounter<T, ThreadSafetyMode::eNotThreadSafe> &&other) { count.store(other.count); other.reset(); }
        TCounter& operator=(TCounter<T, ThreadSafetyMode::eNotThreadSafe> &&other) { count.store(other.count); other.reset(); return *this; }

        /**
         * @brief 自增
         *
         * @return T 自增前的值
         */
        T increase() { return count.fetchIncrease(); }

        /**
         * @brief 自减
         *
         * @return T 自减前的值
         */
        T decrease() { return count.fetchDecrease(); }

        /**
         * @brief 增加一个值
         *
         * @param value 加数
         * @return T 增加前的值
         */
        T add(T value) { return count.fetchAdd(value); }

        /**
         * @brief 减去一个值
         *
         * @param value 减数
         * @return T 被减数
         */
        T sub(T value) { return count.fetchSub(value); }

        /**
         * @brief 获取计数器的值
         *
         * @return T 计数器的值
         */
        T get() const { return count.load(); }

        /**
         * @brief 重置
         *
         */
         void reset() {
            T old = count.load();
            while (!count.compareExchange(old, 0)) {
                Sleep(100);
            }
        }
    private:
        TAtomic<T> count { 0 };
    };

    /**
     * @brief 非线程安全计数器
     *
     * @tparam T
     */
    template <typename T>
    class TCounter<T, ThreadSafetyMode::eNotThreadSafe> {
        template <typename, ThreadSafetyMode>
        friend class TCounter;
    public:
        TCounter() = default;
        TCounter(T count) : count(count) {}
        TCounter(const TCounter &other) : count(other.count) {}
        TCounter& operator=(const TCounter &other) { count = other.count; return *this; }
        TCounter(TCounter &&other) : count(other.count) { other.reset(); }
        TCounter& operator=(TCounter &&other) { count = other.count; other.reset(); return *this; }

        TCounter(const TCounter<T, ThreadSafetyMode::eThreadSafe> &other) : count(other.count.load()) {}
        TCounter& operator=(const TCounter<T, ThreadSafetyMode::eThreadSafe> &other) { count = other.count.load(); return *this; }
        TCounter(TCounter<T, ThreadSafetyMode::eThreadSafe> &&other) : count(other.count.load()) { other.reset(); }
        TCounter& operator=(TCounter<T, ThreadSafetyMode::eThreadSafe> &&other) { count = other.count.load(); other.reset(); return *this; }

        /**
         * @brief 自增
         *
         * @return T 自增前的值
         */
        T increase() { count ++; return count - 1; }

        /**
         * @brief 自减
         *
         * @return T 自减前的值
         */
        T decrease() { count --; return count + 1; }

        /**
         * @brief 增加一个值
         *
         * @param value 加数
         * @return T 增加前的值
         */
        T add(T value) { T result = count; count += value; return result; }

        /**
         * @brief 减去一个值
         *
         * @param value 减数
         * @return T 被减数
         */
        T sub(T value) { T result = count; count -= value; return result; }

        /**
         * @brief 获取计数器的值
         *
         * @return T 计数器的值
         */
        T get() const { return count; }

        /**
         * @brief 重置
         *
         */
         void reset() { count = 0; }
    private:
        T count { 0 };
    };
}
