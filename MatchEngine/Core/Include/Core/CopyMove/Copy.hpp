#pragma once

#include <type_traits>

namespace MatchEngine::Core {
    template <typename T>
    const std::decay_t<T> &Copy(T &&t) {
        return static_cast<const std::decay_t<T> &>(t);
    }

    template <typename T>
    const std::decay_t<T> &Copy(const T &&t) {
        return static_cast<const std::decay_t<T> &>(t);
    }
}
