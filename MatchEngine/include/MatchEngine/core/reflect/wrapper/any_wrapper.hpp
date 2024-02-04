#pragma once

#include <any>
#include <iostream>

namespace MatchEngine {
    // 任意类型包装器
    class AnyWrapper {
    public:
        AnyWrapper() {
            is_void = true;
        }

        AnyWrapper(std::any rhs) {
            ptr = rhs;
            is_const = false;
            is_void = false;
            is_ptr = true;
        }

        template <typename T>
        AnyWrapper (T &&arg) {
            if constexpr (std::is_reference_v<T>) {
                ptr = &arg;
                is_ptr = true;
            } else {
                if constexpr (std::is_copy_assignable_v<std::remove_reference_t<T>>) {
                    ptr = arg;
                    is_ptr = false;
                } else {
                    ptr = &arg;
                    is_ptr = false;
                }
            }
            is_const = std::is_const_v<std::remove_reference_t<T>>;
            is_void = false;
        }

        template <typename T>
        T cast() {
            if constexpr (std::is_void_v<T>) {
                return;
            }
            if (is_void) {
                std::cout << "Convert void to any other type." << std::endl;
                throw std::bad_cast();
            }
            using RealT = std::remove_reference_t<T>;
            using RawT = std::remove_cv_t<RealT>;
            if constexpr (std::is_reference_v<T>) {
                if (is_ptr) {
                    if constexpr (std::is_const_v<RealT>) {
                        if (is_const) {
                            return *std::any_cast<const RawT *>(ptr);
                        }
                        return *std::any_cast<RawT *>(ptr);
                    } else {
                        if (is_const) {
                            std::cout << "Convert const reference to non-const reference." << std::endl;
                            throw std::bad_cast();
                        }
                        return *std::any_cast<RawT *>(ptr);
                    }
                }
                return *std::any_cast<RawT>(&ptr);
            } else {
                if constexpr (std::is_copy_constructible_v<RawT>) {
                    if (is_ptr) {
                        if (is_const) {
                            return *std::any_cast<const RawT *>(ptr);
                        }
                        return *std::any_cast<RawT *>(ptr);
                    }
                    return std::any_cast<RawT>(ptr);
                } else {
                    std::cout << "Convert value without copy constructor." << std::endl;
                    throw std::bad_cast();
                }
            }
        }
    private:
        std::any ptr { nullptr };
        bool is_const;
        bool is_ptr;
        bool is_void;
    };

    template <typename... Args, size_t... Indices>
    std::tuple<Args...> castAnyWrapperArray(std::array<AnyWrapper, sizeof...(Args)> &wrapped_args, std::index_sequence<Indices...>) {
        return std::forward_as_tuple(wrapped_args[Indices].template cast<Args>()...);
    }
}
