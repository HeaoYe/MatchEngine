#pragma once

#include <MatchEngine/core/reflect/reflect_system.hpp>

namespace MatchEngine::Game {
    // 未来编辑器可以通过反射获取组件的信息并编辑
    // RTTI 运行时类型接口
    class RTTI {
        DefaultNoCopyMoveConstruction(RTTI)
    public:
        void setupRTTI() { descriptor = &UserInterface::reflect->getClassByName(getClassName()); }
        virtual std::string getClassName() const { return ""; }
        static std::string GetClassName() { return ""; }
    public:
        template <typename T>
        const T &getValue(const std::string &name) const {
            return descriptor->getMember(name).getValueByPtr<T>(this);
        }

        template <typename T>
        T &getValueMutReference(const std::string &name) {
            return descriptor->getMember(name).getValueMutReferenceByPtr<T>(this);
        }

        template <typename T>
        void setValue(const std::string &name, T &&value) {
            descriptor->getMember(name).setValueByPtr(this, std::forward<T>(value));
        }

        template <typename... Args>
        AnyWrapper invoke(const std::string &name, Args &&... args) {
            return descriptor->getFunction(name).invokeByPtr<Args...>(this, std::forward<Args>(args)...);
        }

        template <typename R, typename... Args>
        R invokeEx(const std::string &name, Args &&... args) {
            return descriptor->getFunction(name).invokeByPtrEx<R, Args...>(this, std::forward<Args>(args)...);
        }
    private:
        const ClassDescriptor *descriptor;
    };

    #define DECLARE_RTTI(class_name) \
    public: \
        std::string getClassName() const override { return #class_name; } \
        static std::string GetClassName() { return #class_name; } \
    private: \

}
