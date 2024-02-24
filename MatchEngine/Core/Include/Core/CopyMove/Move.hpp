#pragma once

#include <type_traits>

namespace MatchEngine::Core {
    template <typename T>
    std::decay_t<T> &&Move(T &&t) noexcept {
        return static_cast<std::decay_t<T> &&>(t);
    };
}
