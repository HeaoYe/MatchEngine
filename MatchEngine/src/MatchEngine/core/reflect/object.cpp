#include <MatchEngine/core/reflect/object.hpp>
#include <MatchEngine/core/reflect/reflect_system.hpp>

namespace MatchEngine {
    bool Object::castTo(const std::string &class_name) {
        if (UserInterface::reflect->hasClass(class_name)) {
            this->descriptor = &UserInterface::reflect->getClassByName(class_name);
            return true;
        }
        return false;
    }

    void Object::forceCastTo(const std::string &class_name) {
        this->descriptor = &UserInterface::reflect->getClassByName(class_name);
    }

    void Object::castTo(const ClassDescriptor &descriptor) {
        this->descriptor = &descriptor;
    }

    Object Object::forceCast(const std::string &class_name) {
        return Object(&UserInterface::reflect->getClassByName(class_name), ptr);
    }

    Object Object::cast(const ClassDescriptor &descriptor) {
        return Object(&descriptor, ptr);
    }
}
