#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/core/reflect/wrapper/any_wrapper.hpp>
#include <functional>
#include <any>

namespace MatchEngine {
    // 成员变量包装器
    class ReflectMemberWrapper {
    public:
        template <class C, typename T>
        ReflectMemberWrapper(T C::*member_ptr) {
            getter_const = [ptr = member_ptr](const void * obj_ptr) {
                return &(static_cast<const C *>(obj_ptr)->*ptr);
            };

            getter_reference = [ptr = member_ptr](void * obj_ptr) {
                return &(static_cast<C *>(obj_ptr)->*ptr);
            };

            setter_copy = [ptr = member_ptr](void * obj_ptr, std::any value) {
                static_cast<C *>(obj_ptr)->*ptr = *std::any_cast<T *>(value);
            };

            setter_copy_const = [ptr = member_ptr](void * obj_ptr, std::any value) {
                static_cast<C *>(obj_ptr)->*ptr = *std::any_cast<const T *>(value);
            };

            setter_move = [ptr = member_ptr](void *obj_ptr, std::any value) {
                static_cast<C *>(obj_ptr)->*ptr = std::move(*std::any_cast<T *>(value));
            };

            setter_value = [ptr = member_ptr](void *obj_ptr, std::any value) {
                static_cast<C *>(obj_ptr)->*ptr = std::any_cast<T>(value);
            };
            
            create_result_const = [](std::any result) {
                return AnyWrapper(*std::any_cast<const T *>(result));
            };
            
            create_result_reference = [](std::any result) {
                return AnyWrapper(*std::any_cast<T *>(result));
            };
        }

        ReflectMemberWrapper(const ReflectMemberWrapper &rhs) {
            this->getter_const = rhs.getter_const;
            this->getter_reference = rhs.getter_reference;
            this->setter_copy = rhs.setter_copy;
            this->setter_copy_const = rhs.setter_copy_const;
            this->setter_move = rhs.setter_move;
            this->setter_value = rhs.setter_value;
            this->create_result_const = rhs.create_result_const;
            this->create_result_reference = rhs.create_result_reference;
        }

        ReflectMemberWrapper(ReflectMemberWrapper &&rhs) {
            this->getter_const = std::move(rhs.getter_const);
            this->getter_reference = std::move(rhs.getter_reference);
            this->setter_copy = std::move(rhs.setter_copy);
            this->setter_copy_const = std::move(rhs.setter_copy_const);
            this->setter_move = std::move(rhs.setter_move);
            this->setter_value = std::move(rhs.setter_value);
            this->create_result_const = std::move(rhs.create_result_const);
            this->create_result_reference = std::move(rhs.create_result_reference);
        }

        template <class C>
        AnyWrapper getValue(const C &obj) const {
            return create_result_const(getter_const(static_cast<const void *>(&obj)));
        }

        template <class C>
        AnyWrapper getValueReference(C &obj) const {
            return create_result_reference(getter_reference(static_cast<void *>(&obj)));
        }

        template <typename T, class C>
        const T &getValue(const C &obj) const {
            return *std::any_cast<const T *>(getter_const(static_cast<const void *>(&obj)));
        }

        template <typename T, class C>
        T &getValueReference(C &obj) const {
            return *std::any_cast<T *>(getter_reference(static_cast<void *>(&obj)));
        }

        template <class C, typename T>
        void setValue(C &obj, T &&value) const {
            if constexpr (std::is_lvalue_reference_v<T>) {
                if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
                    setter_copy_const(static_cast<void *>(&obj), &value);
                } else {
                    setter_copy(static_cast<void *>(&obj), &value);
                }
            } else if constexpr (std::is_rvalue_reference_v<T>) {
                setter_move(static_cast<void *>(&obj), &value);
            } else {
                setter_value(static_cast<void *>(&obj), value);
            }
        }

        AnyWrapper getValueByPtr(const void *obj) const {
            return create_result_const(getter_const(obj));
        }

        AnyWrapper getValueReferenceByPtr(void *obj) const {
            return create_result_reference(getter_reference(obj));
        }

        template <typename T>
        const T &getValueByPtr(const void *obj) const {
            return *std::any_cast<const T *>(getter_const(obj));
        }

        template <typename T>
        T &getValueReferenceByPtr(void *obj) const {
            return *std::any_cast<T *>(getter_reference(obj));
        }

        template <typename T>
        void setValueByPtr(void *obj, T &&value) const {
            if constexpr (std::is_lvalue_reference_v<T>) {
                if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
                    setter_copy_const(obj, &value);
                } else {
                    setter_copy(obj, &value);
                }
            } else if constexpr (std::is_rvalue_reference_v<T>) {
                setter_move(obj, &value);
            } else {
                setter_value(obj, value);
            }
        }
    private:
        std::function<std::any(const void *)> getter_const;
        std::function<std::any(void *)> getter_reference;
        std::function<void(void *, std::any)> setter_copy;
        std::function<void(void *, std::any)> setter_copy_const;
        std::function<void(void *, std::any)> setter_move;
        std::function<void(void *, std::any)> setter_value;
        std::function<AnyWrapper(std::any)> create_result_const;
        std::function<AnyWrapper(std::any)> create_result_reference;
    };
}
