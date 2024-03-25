#pragma once

#include "ThreadSafetyMode.hpp"
#include "ScopeLock.hpp"

#include <mutex>

namespace MatchEngine::Core {
    /**
     * @brief 临界区
     *
     * @tparam EThreadSafetyMode 线程安全模式
     */
    template <EThreadSafetyMode>
    class CriticalSection;

    /**
     * @brief 线程安全的临界区
     *
     */
    template <>
    class CriticalSection<EThreadSafetyMode::eThreadSafe> {
    public:
        CriticalSection() : mutex() {}

        ScopeLock<std::mutex> getScopeLock() const {
            return { &mutex };
        }
    private:
        mutable std::mutex mutex;
    };

    /**
     * @brief 非线程安全的临界区
     *
     * @tparam
     */
    template <>
    class CriticalSection<EThreadSafetyMode::eNotThreadSafe> {
    public:
        ScopeLock<void> getScopeLock() {
            return {};
        }
    };
}
