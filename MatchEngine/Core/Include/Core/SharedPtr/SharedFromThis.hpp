#pragma once

#include "SharedPtrImpl.hpp"
#include "Core/Thread/ThreadSafetyMode.hpp"
#include "Core/Thread/Counter.hpp"

namespace MatchEngine::Core {
    /**
     * @brief 从自己共享
     *
     * @tparam T "自己"的类型
     * @tparam mode 线程安全模式
     */
    template <typename T, ThreadSafetyMode mode = ThreadSafetyMode::eThreadSafe>
    class TSharedFromThis {
    public:
        TSharedFromThis() : reference_count(new TCounter<size_t, mode>(1)) {}

        virtual ~TSharedFromThis() {
            assert(reference_count->decrease() == 1);
        }

        /**
         * @brief 共享自己
         *
         * @return SharedPtr<T, mode> 生成的共享指针
         */
        TSharedPtr<T, mode> shareMe() { return TSharedPtr<T, mode>(reinterpret_cast<T *>(this), reference_count); }
    private:
        TCounter<size_t, mode> *reference_count;
    };

}
