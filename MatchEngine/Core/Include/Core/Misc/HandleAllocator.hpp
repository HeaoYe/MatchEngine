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
    class THandleAllocator;

    /**
     * @brief 线程安全的句柄分配器
     *
     * @tparam HandleType 句柄类型
     */
    template <typename HandleType>
    class THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> {
        template <typename _HandleType, ThreadSafetyMode _mode>
        friend class THandleAllocator;
    public:
        inline static const HandleType InvalidHandle = HandleType(0);
    public:
        THandleAllocator() : current_handle(InvalidHandle) {}
        THandleAllocator(const THandleAllocator &other) : current_handle(other.current_handle.load()) {}
        THandleAllocator(THandleAllocator &&other) : current_handle(other.current_handle.load()) {
            other.current_handle.exchange(InvalidHandle);
        }
        DefineDefaultAssignmentOperator(THandleAllocator)

        THandleAllocator(const THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> &other) : current_handle(other.current_handle) {}
        THandleAllocator(THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> &&other) : current_handle(other.current_handle) {
            other.current_handle = THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe>::InvalidHandle;
        }
        THandleAllocator &operator=(const THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> &other) {
            this->current_handle.exchange(other.current_handle);
            return *this;
        }
        THandleAllocator &operator=(THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> &&other) {
            this->current_handle.exchange(other.current_handle);
            other.current_handle = THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe>::InvalidHandle;
            return *this;
        }
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
    class THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> {
        template <typename _HandleType, ThreadSafetyMode _mode>
        friend class THandleAllocator;
    public:
        inline static const HandleType InvalidHandle = HandleType(0);
    public:
        THandleAllocator() : current_handle(InvalidHandle) {}
        THandleAllocator(const THandleAllocator &other) : current_handle(other.current_handle) {}
        THandleAllocator(THandleAllocator &&other) : current_handle(other.current_handle) {
            other.current_handle = InvalidHandle;
        }
        DefineDefaultAssignmentOperator(THandleAllocator)

        THandleAllocator(const THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> &other) : current_handle(other.current_handle.load()) {}
        THandleAllocator(THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> &&other) : current_handle(other.current_handle.load()) {
            other.current_handle.exchange(InvalidHandle);
        }
        THandleAllocator &operator=(const THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> &other) {
            this->current_handle = other.current_handle.load();
            return *this;
        }
        THandleAllocator &operator=(THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> &&other) {
            this->current_handle = other.current_handle.load();
            other.current_handle.exchange(THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe>::InvalidHandle);
            return *this;
        }
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
