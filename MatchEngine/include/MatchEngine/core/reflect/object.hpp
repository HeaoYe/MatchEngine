#pragma once

#include <MatchEngine/core/reflect/wrapper/any_wrapper.hpp>
#include <MatchEngine/core/reflect/class_descriptor.hpp>

namespace MatchEngine {
    // Object 就是 Object
    class Object {
    public:
        template <class C>
        Object(const ClassDescriptor *descriptor, C *raw_ptr) : descriptor(descriptor), ptr(raw_ptr) {
            delete_callback = [raw_ptr]() {
                delete raw_ptr;
            };
        }

        ~Object() {
            if (ptr != nullptr) {
                delete_callback();
                ptr = nullptr;
            }
        }

        AnyWrapper getValue(const std::string &name) {
            return std::move(descriptor->getMember(name).getValueByPtr(ptr));
        }

        AnyWrapper getValueReference(const std::string &name) {
            return std::move(descriptor->getMember(name).getValueReferenceByPtr(ptr));
        }
        
        template <typename T>
        T getValue(const std::string &name) {
            return descriptor->getMember(name).getValueByPtr<T>(ptr);
        }

        template <typename T>
        T &getValueReference(const std::string &name) {
            return descriptor->getMember(name).getValueReferenceByPtr<T>(ptr);
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
    private:
        const ClassDescriptor *descriptor;
        void *ptr;
        std::function<void()> delete_callback;
    };
}
