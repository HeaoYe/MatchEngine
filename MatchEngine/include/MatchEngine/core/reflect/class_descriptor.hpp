#pragma once

#include <MatchEngine/core/reflect/wrapper/member_wrapper.hpp>
#include <MatchEngine/core/reflect/wrapper/function_wrapper.hpp>
#include <map>

namespace MatchEngine {
    // 类描述符, 由一个反射类型的 多个成员变量包装器 和 多个成员函数包装器 组成
    class ClassDescriptor {
        NoCopyMoveConstruction(ClassDescriptor)
        template <class C>
        friend class ClassBuilder;
    public:
        ClassDescriptor(const std::string &name);

        const ReflectMemberWrapper &getMember(const std::string &name) const;
        const ReflectFunctionWrapper &getFunction(const std::string &name) const;
        
        const std::vector<std::string> &getMemberNames() const;
        const std::vector<std::string> &getFunctionNames() const;

        class Object *createObject() const;
        class Object *createObject(void *ptr) const;

        void deleteClass(void *ptr) const;
        void deleteObject(class Object *ptr) const;
    private:
        std::string name;
        std::map<std::string, ReflectMemberWrapper> members;
        std::map<std::string, ReflectFunctionWrapper> functions;
        std::vector<std::string> member_names;
        std::vector<std::string> function_names;
        std::function<void *()> create_object_callback;
        std::function<void *(void *)> create_object_by_ptr_callback;
        std::function<void(void *)> delete_callback;
    };
}
