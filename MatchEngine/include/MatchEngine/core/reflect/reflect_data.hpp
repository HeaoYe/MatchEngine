#pragma once

#include <MatchEngine/core/reflect/class_descriptor.hpp>

namespace MatchEngine {
    // 存储全局类描述符
    extern std::map<std::string, ClassDescriptor *> classes;
}
