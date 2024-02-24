#pragma once

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
     * @brief 不可用句柄
     *
     * @tparam HandleType 句柄类型
     */
    template <typename HandleType>
    struct InvalidHandleTrait {
        inline static constexpr HandleType value { 0 };
    };

    /**
     * @brief 线程安全的句柄分配器
     *
     * @tparam HandleType 句柄类型
     */
    template <typename HandleType>
    class THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> {
        template <typename, ThreadSafetyMode>
        friend class THandleAllocator;
    public:
        THandleAllocator() : current_handle(InvalidHandleTrait<HandleType>::value) {}
        THandleAllocator(const THandleAllocator &other) : current_handle(other.current_handle.load()) {}
        THandleAllocator(THandleAllocator &&other) : current_handle(other.current_handle.load()) {
            other.current_handle.exchange(InvalidHandleTrait<HandleType>::value);
        }
        THandleAllocator &operator=(const THandleAllocator &other) {
            this->current_handle.exchange(other.current_handle.load());
            return *this;
        }
        THandleAllocator &operator=(THandleAllocator &&other) {
            this->current_handle.exchange(other.current_handle.load());
            other.current_handle.exchange(InvalidHandleTrait<HandleType>::value);
            return *this;
        }

        THandleAllocator(const THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> &other) : current_handle(other.current_handle) {}
        THandleAllocator(THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> &&other) : current_handle(other.current_handle) {
            other.current_handle = InvalidHandleTrait<HandleType>::value;
        }
        THandleAllocator &operator=(const THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> &other) {
            this->current_handle.exchange(other.current_handle);
            return *this;
        }
        THandleAllocator &operator=(THandleAllocator<HandleType, ThreadSafetyMode::eNotThreadSafe> &&other) {
            this->current_handle.exchange(other.current_handle);
            other.current_handle = InvalidHandleTrait<HandleType>::value;
            return *this;
        }
    public:
        HandleType allocate() {
            HandleType result = ++current_handle;
            while (result == InvalidHandleTrait<HandleType>::value) {
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
        template <typename, ThreadSafetyMode>
        friend class THandleAllocator;
    public:
        THandleAllocator() : current_handle(InvalidHandleTrait<HandleType>::value) {}
        THandleAllocator(const THandleAllocator &other) : current_handle(other.current_handle) {}
        THandleAllocator(THandleAllocator &&other) : current_handle(other.current_handle) {
            other.current_handle = InvalidHandleTrait<HandleType>::value;
        }
        THandleAllocator &operator=(const THandleAllocator &other) {
            this->current_handle = other.current_handle;
            return *this;
        }
        THandleAllocator &operator=(THandleAllocator &&other) {
            this->current_handle = other.current_handle;
            other.current_handle = InvalidHandleTrait<HandleType>::value;
            return *this;
        }

        THandleAllocator(const THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> &other) : current_handle(other.current_handle.load()) {}
        THandleAllocator(THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> &&other) : current_handle(other.current_handle.load()) {
            other.current_handle.exchange(InvalidHandleTrait<HandleType>::value);
        }
        THandleAllocator &operator=(const THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> &other) {
            this->current_handle = other.current_handle.load();
            return *this;
        }
        THandleAllocator &operator=(THandleAllocator<HandleType, ThreadSafetyMode::eThreadSafe> &&other) {
            this->current_handle = other.current_handle.load();
            other.current_handle.exchange(InvalidHandleTrait<HandleType>::value);
            return *this;
        }
    public:
        HandleType allocate() {
            HandleType result = ++current_handle;
            if (result == InvalidHandleTrait<HandleType>::value) {
                result = ++current_handle;
            }
            return result;
        }
    private:
        HandleType current_handle;
    };
}
