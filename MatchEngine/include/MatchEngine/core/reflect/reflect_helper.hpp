#pragma once

#include <MatchEngine/core/reflect/class_builder.hpp>

namespace MatchEngine::ReflectHelper {
    // 注册一个反射类
    template <class C>
    ClassBuilder<C> AddClass(const std::string &name) {
        return std::move(ClassBuilder<C>(name));
    }

    // 通过注册时的名字 获取反射类的类描述符
    const ClassDescriptor &GetClassByName(const std::string &name);

    // 销毁所有注册的反射类
    void DestroyReflectHelper();
}
