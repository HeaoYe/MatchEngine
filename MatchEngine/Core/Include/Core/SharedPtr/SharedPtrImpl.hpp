#pragma once

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
}
