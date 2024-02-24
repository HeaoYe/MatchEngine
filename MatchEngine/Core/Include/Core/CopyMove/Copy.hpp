#pragma once

#include <type_traits>

namespace MatchEngine::Core {
    template <typename T>
    const std::decay_t<T> &Copy(T &t) noexcept {
        return static_cast<const std::decay_t<T> &>(t);
    }

    template <typename T>
    const std::decay_t<T> &Copy(const T &t) noexcept {
        return static_cast<const std::decay_t<T> &>(t);
    }

    template <typename T>
    const std::decay_t<T> &Copy(T &&t) noexcept {
        if constexpr (std::is_rvalue_reference_v<T &&>) {
            static_assert(0, "Copy: 不可以复制右值引用");
        }
        return static_cast<const std::decay_t<T> &>(t);
    }
}
