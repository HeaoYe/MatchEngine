#pragma once

namespace MatchEngine::Core {
    /**
     * @brief 线程安全模式, 用于区分一个类的两种版本
     *
     */
    enum class ThreadSafetyMode {
        eThreadSafe,
        eNotThreadSafe,
    };
}
