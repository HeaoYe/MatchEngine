#pragma once

#include "Core/Misc/Forward.hpp"
#include "Core/Thread/ThreadSafetyMode.hpp"
#include "Core/Thread/Counter.hpp"

namespace MatchEngine::Core {
    /**
     * @brief 智能指针
     *
     * @tparam T 指针类型
     * @tparam mode 线程安全模式
     */
    template <typename T, EThreadSafetyMode mode>
    class TSharedPtr {
        template <typename, EThreadSafetyMode>
        friend class TSharedPtr;

        template <typename, EThreadSafetyMode>
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

        template <EThreadSafetyMode OtherMode>
        TSharedPtr(const TSharedPtr<T, OtherMode> &other) : object(other.object), reference_count(other.reference_count) {
            increaseRef();
        }

        template <EThreadSafetyMode OtherMode>
        TSharedPtr &operator=(const TSharedPtr<T, OtherMode> &other) {
            object = other.object;
            reference_count = other.reference_count;
            increaseRef();
            return *this;
        }

        template <EThreadSafetyMode OtherMode>
        TSharedPtr(TSharedPtr<T, OtherMode> &&other) : object(other.object), reference_count(other.reference_count) {
            other.object = 0;
            other.reference_count = nullptr;
        }

        template <EThreadSafetyMode OtherMode>
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

    template <typename T, EThreadSafetyMode mode = EThreadSafetyMode::eThreadSafe>
    TSharedPtr<T, mode> MakeSharedPtr(T *ptr) {
        return TSharedPtr<T, mode>(ptr);
    }

    template <typename T, typename ...Args, EThreadSafetyMode mode = EThreadSafetyMode::eThreadSafe>
    TSharedPtr<T, mode> MakeSharedPtr(Args &&...args) {
        return MakeSharedPtr(new T(Forward<Args>(args)...));
    }
}
