#pragma once

#include <MatchEngine/core/reflect/wrapper/any_wrapper.hpp>
#include <MatchEngine/core/reflect/class_descriptor.hpp>

namespace MatchEngine {
    // Object 就是 Object
    class Object {
    public:
        Object(const ClassDescriptor *descriptor, const void *raw_ptr, bool control_ptr = true) : descriptor(descriptor), ptr(const_cast<void *>(raw_ptr)), control_ptr(control_ptr) {}

        ~Object() {
            if (ptr != nullptr && control_ptr) {
                descriptor->deleteClass(ptr);
                ptr = nullptr;
            }
        }

        bool castTo(const std::string &class_name);
        void forceCastTo(const std::string &class_name);
        void castTo(const ClassDescriptor &descriptor);

        Object forceCast(const std::string &class_name);
        Object cast(const ClassDescriptor &descriptor);

        AnyWrapper getValue(const std::string &name) const {
            return std::move(descriptor->getMember(name).getValueByPtr(ptr));
        }

        AnyWrapper getValueMutReference(const std::string &name) {
            return std::move(descriptor->getMember(name).getValueMutReferenceByPtr(ptr));
        }

        template <typename T>
        const T &getValue(const std::string &name) const {
            return descriptor->getMember(name).getValueByPtr<T>(ptr);
        }

        template <typename T>
        T &getValueMutReference(const std::string &name) {
            return descriptor->getMember(name).getValueMutReferenceByPtr<T>(ptr);
        }

        template <typename T>
        void setValue(const std::string &name, T &&value) {
            descriptor->getMember(name).setValueByPtr(ptr, std::forward<T>(value));
        }

        template <typename... Args>
        AnyWrapper invoke(const std::string &name, Args &&... args) const {
            return descriptor->getFunction(name).invokeByPtr<Args...>(ptr, std::forward<Args>(args)...);
        }

        template <typename R, typename... Args>
        R invokeEx(const std::string &name, Args &&... args) const {
            return descriptor->getFunction(name).invokeByPtrEx<R, Args...>(ptr, std::forward<Args>(args)...);
        }

        template <class C>
        C *release() {
            void *raw_ptr = ptr;
            ptr = nullptr;
            return static_cast<C *>(raw_ptr);
        }

        void release() {
            ptr = nullptr;
        }
    private:
        const ClassDescriptor *descriptor;
        void *ptr;
        bool control_ptr;
    };
}
