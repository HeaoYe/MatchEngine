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
    class TSharedPtr {
        template <typename, ThreadSafetyMode>
        friend class TSharedPtr;

        template <typename, ThreadSafetyMode>
        friend class TSharedFromThis;
    private:
        TSharedPtr(T *object, TCounter<size_t, mode> *reference_count) : object(object), reference_count(reference_count) {
            increaseRef();
        }
    public:
        TSharedPtr(T *object) : object(object), reference_count(new TCounter<size_t, mode>(0)) {
            increaseRef();
        }

        ~TSharedPtr() {
            if (reference_count != nullptr) {
                decreaseRef();
            }
        }

        template <ThreadSafetyMode OtherMode>
        TSharedPtr(const TSharedPtr<T, OtherMode> &other) : object(other.object), reference_count(other.reference_count) {
            increaseRef();
        }

        template <ThreadSafetyMode OtherMode>
        TSharedPtr &operator=(const TSharedPtr<T, OtherMode> &other) {
            object = other.object;
            reference_count = other.reference_count;
            increaseRef();
            return *this;
        }

        template <ThreadSafetyMode OtherMode>
        TSharedPtr(TSharedPtr<T, OtherMode> &&other) : object(other.object), reference_count(other.reference_count) {
            other.object = 0;
            other.reference_count = nullptr;
        }

        template <ThreadSafetyMode OtherMode>
        TSharedPtr &operator=(TSharedPtr<T, OtherMode> &&other) {
            object = other.object;
            reference_count = other.reference_count;
            other.object = 0;
            other.reference_count = nullptr;
            return *this;
        }

        void reset() {
            decreaseRef();
            object = nullptr;
            reference_count = nullptr;
        }

        T *operator->() { return object; }

        const T *operator->() const { return object; }
    private:
        void increaseRef() {
            reference_count->increase();
        }

        void decreaseRef() {
            if (reference_count->decrease() == 1) {
                delete object;
                object = nullptr;
            }
        }
    private:
        T *object;
        TCounter<size_t, mode> *reference_count;
    };

    template <typename T, ThreadSafetyMode mode = ThreadSafetyMode::eThreadSafe>
    TSharedPtr<T, mode> MakeSharedPtr(T *ptr) {
        return TSharedPtr<T, mode>(ptr);
    }

    template <typename T, typename ...Args, ThreadSafetyMode mode = ThreadSafetyMode::eThreadSafe>
    TSharedPtr<T, mode> MakeSharedPtr(Args &&...args) {
        return MakeSharedPtr(new T(Forward<Args>(args)...));
    }
}
