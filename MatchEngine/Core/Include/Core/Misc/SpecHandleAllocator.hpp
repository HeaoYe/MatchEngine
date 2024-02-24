#pragma once

#include "HandleAllocator.hpp"

#include <cstdint>

namespace MatchEngine::Core {
    /**
     * @brief 32位无符号整数句柄分配器
     *
     * @tparam mode 线程安全模式
     */
    template <ThreadSafetyMode mode = ThreadSafetyMode::eThreadSafe>
    using TU32HandleAllocator = THandleAllocator<uint32_t, mode>;

    /**
     * @brief 64位无符号整数句柄分配器
     *
     * @tparam mode 线程安全模式
     */
    template <ThreadSafetyMode mode = ThreadSafetyMode::eThreadSafe>
    using TU64HandleAllocator = THandleAllocator<uint64_t, mode>;

    /**
     * @brief 无符号整数句柄分配器
     *
     * @tparam mode 线程安全模式
     */
    template <ThreadSafetyMode mode = ThreadSafetyMode::eThreadSafe>
    using TUIntHandleAllocator = THandleAllocator<size_t, mode>;
}
