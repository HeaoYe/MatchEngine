#pragma once

#include "Core/Misc/Forward.hpp"
#include "Core/Thread/ThreadSafetyMode.hpp"
#include "Core/Thread/Counter.hpp"

namespace MatchEngine::Core {
    // template <typename T, ThreadSafetyMode mode>
    // class SharedPtr;

    /**
     * @brief 智能指针
     *
     * @tparam T 指针类型
     *
     * TODO: 不依赖std::shared_ptr实现
     */
    template <typename T, ThreadSafetyMode mode>
    class SharedPtr {
    public:
        SharedPtr(T *data) : data(data), reference_count(new Counter<size_t, mode>(0)) {
            increaseRef();
        }

        ~SharedPtr() {
            if (reference_count != nullptr) {
                decreaseRef();
            }
        }

        template <ThreadSafetyMode OtherMode>
        SharedPtr(const SharedPtr<T, OtherMode> &other) : data(other.data), reference_count(other.reference_count) {
            increaseRef();
        }

        template <ThreadSafetyMode OtherMode>
        SharedPtr &operator=(const SharedPtr<T, OtherMode> &other) {
            data = other.data;
            reference_count = other.reference_count;
            increaseRef();
        }

        template <ThreadSafetyMode OtherMode>
        SharedPtr(SharedPtr<T, OtherMode> &&other) : data(other.data), reference_count(other.reference_count) {
            other.data = 0;
            other.reference_count = nullptr;
        }

        template <ThreadSafetyMode OtherMode>
        SharedPtr &operator=(SharedPtr<T, OtherMode> &&other) {
            data = other.data;
            reference_count = other.reference_count;
            other.data = 0;
            other.reference_count = nullptr;
        }

        void reset() {
            decreaseRef();
            data = nullptr;
            reference_count = nullptr;
        }

        T *operator->() { return data; }

        const T *operator->() const { return data; }
    private:
        void increaseRef() {
            reference_count->increase();
        }

        void decreaseRef() {
            if (reference_count->decrease() == 1) {
                delete data;
                data = nullptr;
            }
        }
    private:
        T *data;
        Counter<size_t, mode> *reference_count;
    };

    template <typename T, ThreadSafetyMode mode = ThreadSafetyMode::eThreadSafe>
    SharedPtr<T, mode> MakeSharedPtr(T *ptr) {
        return SharedPtr<T, mode>(ptr);
    }

    template <typename T, typename ...Args, ThreadSafetyMode mode = ThreadSafetyMode::eThreadSafe>
    SharedPtr<T, mode> MakeSharedPtr(Args &&...args) {
        return MakeSharedPtr(new T(Forward<Args>(args)...));
    }
}
