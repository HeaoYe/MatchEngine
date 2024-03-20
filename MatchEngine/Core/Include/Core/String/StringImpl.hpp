#pragma once

#include <string>

namespace MatchEngine::Core {
    /**
     * @brief 字符串类
     *
     * TODO: 不依赖std::string实现
     */
    class String : std::string {
    public:
        String() : std::string() {}

        String(const char *str) : std::string(str) {}

        String(std::string &&str) : std::string(str) {}

        const char *asCPtr() const {
            return std::string::c_str();
        }
    };
}
