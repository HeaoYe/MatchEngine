#pragma once

#include "Core/Misc/Forward.hpp"

#include <memory>

namespace MatchEngine::Core {
    /**
     * @brief 智能指针
     *
     * @tparam T 指针类型
     *
     * TODO: 不依赖std::shared_ptr实现
     */
    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T>
    SharedPtr<T> MakeSharedPtr(T *ptr) {
        return SharedPtr<T>(ptr);
    }

    template <typename T, typename ...Args>
    SharedPtr<T> MakeSharedPtr(Args &&...args) {
        return MakeSharedPtr(new T(Forward<Args>(args)...));
    }
}
