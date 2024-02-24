#pragma once

#include "Core/Defines.hpp"
#include "Core/Thread/ThreadSafetyMode.hpp"

#include <atomic>

namespace MatchEngine::Core {
    /**
     * @brief 句柄分配器
     *
     * @tparam HandleType 句柄类型
     * @tparam ThreadSafetyMode 线程安全模式
     */
    template <typename HandleType, ThreadSafetyMode>
    class HandleAllocator;

    /**
     * @brief 线程安全的句柄分配器
     *
     * @tparam HandleType 句柄类型
     */
    template <typename HandleType>
    class HandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> {
    public:
        inline static const HandleType InvalidHandle = HandleType(0);
    public:
        HandleAllocator() : current_handle(InvalidHandle) {}
        HandleAllocator(const HandleAllocator &other) : current_handle(other.current_handle.load()) {}
        HandleAllocator(HandleAllocator &&other) : current_handle(other.current_handle.load()) {
            other.current_handle.exchange(InvalidHandle);
        }
        DefineDefaultAssignmentOperator(HandleAllocator)
    public:
        HandleType allocate() {
            HandleType result = ++current_handle;
            while (result == InvalidHandle) {
                result = ++current_handle;
            }
            return result;
        }
    private:
        std::atomic<HandleType> current_handle;
    };

    /**
     * @brief 非线程安全的句柄分配器
     *
     * @tparam HandleType 句柄类型
     */
    template <typename HandleType>
    class HandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> {
    public:
        inline static const HandleType InvalidHandle = HandleType(0);
    public:
        HandleAllocator() : current_handle(InvalidHandle) {}
        HandleAllocator(const HandleAllocator &other) : current_handle(other.current_handle) {}
        HandleAllocator(HandleAllocator &&other) : current_handle(other.current_handle) {
            other.current_handle = InvalidHandle;
        }
        DefineDefaultAssignmentOperator(HandleAllocator)
    public:
        HandleType allocate() {
            HandleType result = ++current_handle;
            if (result == InvalidHandle) {
                result = ++current_handle;
            }
            return result;
        }
    private:
        HandleType current_handle;
    };
}
