#pragma once

namespace MatchEngine::Core {
    /**
     * @brief 线程安全模式, 用于区分一个类的两种版本
     *
     * 用法: template <..., EThreadSafetyMode mode>
     *
     */
    enum class EThreadSafetyMode {
        eThreadSafe,
        eNotThreadSafe,
    };

    /**
     * @brief 线程安全结模式构体, 线程安全
     *
     * 用法: template <..., typename ...Ts, typename ThreadSafetyModeStruct>
     *
     */
    struct ThreadSafetyModeThreadSafeStruct {
        static constexpr EThreadSafetyMode SafetyMode = EThreadSafetyMode::eThreadSafe;
    };

    /**
     * @brief 线程安全结模式构体, 非线程安全
     *
     * 用法: template <..., typename ...Ts, typename ThreadSafetyModeStruct>
     *
     */
    struct ThreadSafetyModeNotThreadSafeStruct {
        static constexpr EThreadSafetyMode SafetyMode = EThreadSafetyMode::eNotThreadSafe;
    };
}
