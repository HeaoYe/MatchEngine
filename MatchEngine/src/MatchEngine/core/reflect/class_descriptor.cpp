#include <MatchEngine/core/reflect/class_descriptor.hpp>
#include <MatchEngine/core/reflect/object.hpp>
#include <MatchEngine/core/reflect/object.hpp>
#include <MatchEngine/core/logger/logger.hpp>

namespace MatchEngine {
    // 类描述符具体实现, 获取反射类型信息

    ClassDescriptor::ClassDescriptor(const std::string &name) : name(name) {}
    
    const ReflectMemberWrapper &ClassDescriptor::getMember(const std::string &name) const {
        if (members.find(name) == members.end()) {
            MCH_CORE_ERROR("No member named {} in class {}.", name, this->name)
        }
        return members.at(name);
    }
    
    const ReflectFunctionWrapper &ClassDescriptor::getFunction(const std::string &name) const {
        if (functions.find(name) == functions.end()) {
            MCH_CORE_ERROR("No function named {} in class {}.", name, this->name)
        }
        return functions.at(name);
    }
        
    const std::vector<std::string> &ClassDescriptor::getMemberNames() const {
        return member_names;
    }
    
    const std::vector<std::string> &ClassDescriptor::getFunctionNames() const {
        return function_names;
    }
    
    Object *ClassDescriptor::createObject() const {
        return static_cast<Object *>(create_object_callback());
    }

    Object *ClassDescriptor::createObject(void *ptr) const {
        return static_cast<Object *>(create_object_by_ptr_callback(ptr));
    }

    void ClassDescriptor::deleteClass(void *ptr) const {
        delete_callback(ptr);
    }

    void ClassDescriptor::deleteObject(Object *ptr) const {
        delete ptr;
    }
}
