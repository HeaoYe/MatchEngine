#pragma once

#include "Core/Defines.hpp"

namespace MatchEngine::Core {
    /**
     * @brief 作用域锁, 在构造函数上锁, 离开作用域析构时解锁
     *
     * @tparam MutexType 互斥量类型
     */
    template <typename MutexType>
    class ScopeLock {
    public:
        ScopeLock(MutexType *mutex) : mutex(mutex) {
            this->mutex->lock();
        }
        ScopeLock(const ScopeLock &) = delete;
        ScopeLock(ScopeLock &&other) {
            this->mutex = other.mutex;
            other.mutex = nullptr;
        }
        DefineDefaultAssignmentOperator(ScopeLock)

        ~ScopeLock() {
            if (this->mutex != nullptr) {
                this->mutex->unlock();
            }
        }
    private:
        MutexType *mutex { nullptr };
    };

    /**
     * @brief 空类型作用域锁, 不具备锁的功能
     *
     */
    template <>
    class ScopeLock<void> {};
}
