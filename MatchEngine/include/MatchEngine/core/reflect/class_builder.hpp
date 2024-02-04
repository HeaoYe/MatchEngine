#pragma once

#include <MatchEngine/core/reflect/class_descriptor.hpp>
#include <MatchEngine/core/reflect/reflect_data.hpp>
#include <MatchEngine/core/reflect/object.hpp>

namespace MatchEngine {
    // 类描述符构建者, 由ClassBuilder类构建ClassDescriptor
    template <class C>
    class ClassBuilder {
        __NoCopyConstruction(ClassBuilder)
    public:
        ClassBuilder(const std::string &name) {
            descriptor = new ClassDescriptor(name);

            if constexpr (std::is_default_constructible_v<C>) {
                descriptor->create_object_callback = [descriptor_ptr = descriptor]() {
                    return new Object(descriptor_ptr, new C());
                };
            } else {
                descriptor->create_object_callback = [descriptor_ptr = descriptor]() {
                    C *ptr = nullptr;
                    return new Object(descriptor_ptr, ptr);
                };
            }

            descriptor->create_object_by_ptr_callback = [descriptor_ptr = descriptor](void *ptr) {
                return new Object(descriptor_ptr, static_cast<C *>(ptr));
            };
        }

        ~ClassBuilder() {
            if (descriptor == nullptr) {
                return;
            }
            if (classes.find(descriptor->name) == classes.end()) {
                classes.insert(std::make_pair(descriptor->name, descriptor));
            } else {
                std::cout << "Class " << descriptor->name << " has been added." << std::endl;
                delete descriptor;
            }
        }

        ClassBuilder(ClassBuilder &&rhs) {
            this->descriptor = rhs.descriptor;
            rhs.descriptor = nullptr;
        }

        template <typename T>
        ClassBuilder &addMember(const std::string &name, T C::*member) {
            if (descriptor->members.find(name) == descriptor->members.end()) {
                descriptor->members.insert(std::make_pair(name, ReflectMemberWrapper(member)));
                descriptor->member_names.push_back(name);
            } else {
                std::cout << "Member " << name << " has been added." << std::endl;
            }
            return *this;
        }

        template <typename T>
        ClassBuilder &addFunction(const std::string &name, T func) {
            if (descriptor->functions.find(name) == descriptor->functions.end()) {
                descriptor->functions.insert(std::make_pair(name, ReflectFunctionWrapper(func)));
                descriptor->function_names.push_back(name);
            } else {
                std::cout << "Function " << name << " has been added." << std::endl;
            }
            return *this;
        }

        ClassBuilder &setCreateObjectCallback(std::function<C *()> create_object_callback) {
            descriptor->create_object_callback = [descriptor_ptr = descriptor, create_object_callback_copy = create_object_callback]() {
                auto ptr = create_object_callback_copy();
                return new Object(descriptor_ptr, ptr);
            };
            return *this;
        }
    private:
        ClassDescriptor *descriptor = nullptr;
    };
}
