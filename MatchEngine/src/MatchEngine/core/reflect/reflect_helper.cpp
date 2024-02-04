#include <MatchEngine/core/reflect/reflect_helper.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine::ReflectHelper {
    static ClassDescriptor *empty = nullptr;

    const ClassDescriptor &GetClassByName(const std::string &name) {
        if (classes.find(name) == classes.end()) {
            MCH_CORE_ERROR("No class named {}.", name);
            return *empty;
        }
        return *classes.at(name);
    }

    void DestroyReflectHelper() {
        for (auto &[name, descriptor] : classes) {
            delete descriptor;
        }
        classes.clear();
    }
}
