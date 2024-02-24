#pragma once

#include <type_traits>

namespace MatchEngine::Core {
    template<typename T>
    T &&Forward(std::remove_reference_t<T> &t) noexcept {
        return static_cast<T &&>(t);
    }

    template<typename T>
    T &&Forward(std::remove_reference_t<T> &&t) noexcept {
      static_assert(!std::is_lvalue_reference_v<T>, "Forward: 禁止将右值转换为左值");
      return static_cast<T &&>(t);
    }
}
