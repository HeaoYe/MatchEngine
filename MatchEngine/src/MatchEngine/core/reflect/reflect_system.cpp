#include <MatchEngine/core/reflect/reflect_system.hpp>
#include <MatchEngine/core/reflect/reflect_macro.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine {
    ReflectSystem::ReflectSystem() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;
    }

    ReflectSystem::~ReflectSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        for (auto &[name, descriptor] : classes) {
            delete descriptor;
        }
        classes.clear();
    }

    void ReflectSystem::registerReflectProperties() {
        MatchEngineParser_RegisterReflectProperties();
    }

    bool ReflectSystem::hasClass(const std::string &name) {
        return classes.find(name) != classes.end();
    }

    const ClassDescriptor &ReflectSystem::getClassByName(const std::string &name) {
        if (!hasClass(name)) {
            MCH_CORE_ERROR("No class named {}.", name);
            static ClassDescriptor *empty = nullptr;
            return *empty;
        }
        return *classes.at(name);
    }
}
