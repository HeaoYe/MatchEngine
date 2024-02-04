#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/core/reflect/wrapper/any_wrapper.hpp>
#include <functional>

namespace MatchEngine {
    // 成员函数包装器
    class ReflectFunctionWrapper {
    public:
        template <class C, typename... Args>
        ReflectFunctionWrapper(void (C::*func)(Args ...)) {
            callback = [func_ptr = func](void *obj_ptr, std::any args) {
                auto wrapped_args = std::any_cast<std::array<AnyWrapper, sizeof...(Args) + 1> *>(args);
                (*wrapped_args)[0] = AnyWrapper(*static_cast<C *>(obj_ptr));
                std::tuple<C &, Args...> call_args = 
                castAnyWrapperArray<C &, Args...>(
                    *wrapped_args,
                    std::make_index_sequence<sizeof...(Args) + 1> {}
                );
                std::apply(
                    func_ptr,
                    call_args
                );
                return std::any();
            };

            create_result = [](std::any) {
                return AnyWrapper();
            };
        }

        template <typename R, class C, typename... Args>
        ReflectFunctionWrapper(R (C::*func)(Args ...)) {
            callback = [func_ptr = func](void *obj_ptr, std::any args) {
                auto wrapped_args = std::any_cast<std::array<AnyWrapper, sizeof...(Args) + 1> *>(args);
                (*wrapped_args)[0] = AnyWrapper(*static_cast<C *>(obj_ptr));
                std::tuple<C &, Args...> call_args = 
                castAnyWrapperArray<C &, Args...>(
                    *wrapped_args,
                    std::make_index_sequence<sizeof...(Args) + 1> {}
                );
                return std::any(std::apply(func_ptr, call_args));
            };
            
            create_result = [](std::any result) {
                return AnyWrapper(std::any_cast<R>(result));
            };
        }

        ReflectFunctionWrapper(const ReflectFunctionWrapper &rhs) {
            this->callback = rhs.callback;
            this->create_result = rhs.create_result;
        }

        ReflectFunctionWrapper(ReflectFunctionWrapper &&rhs) {
            this->callback = std::move(rhs.callback);
            this->create_result = std::move(rhs.create_result);
        }

        template <class C, typename... Args>
        AnyWrapper invoke(C &obj, Args &&... args) const {
            std::array<AnyWrapper, sizeof...(Args) + 1> wrapped_args = {
                AnyWrapper(),
                AnyWrapper(std::forward<Args>(args))...
            };
            return create_result(callback(&obj, &wrapped_args));
        }

        template <typename R, class C, typename... Args>
        R invokeEx(C &obj, Args &&... args) const {
            std::array<AnyWrapper, sizeof...(Args) + 1> wrapped_args = {
                AnyWrapper(),
                AnyWrapper(std::forward<Args>(args))...
            };
            return std::any_cast<R>(callback(&obj, &wrapped_args));
        }

        template <typename... Args>
        AnyWrapper invokeByPtr(void *obj, Args &&... args) const {
            std::array<AnyWrapper, sizeof...(Args) + 1> wrapped_args = {
                AnyWrapper(),
                AnyWrapper(std::forward<Args>(args))...
            };
            return create_result(callback(obj, &wrapped_args));
        }

        template <typename R, typename... Args>
        R invokeByPtrEx(void *obj, Args &&... args) const {
            std::array<AnyWrapper, sizeof...(Args) + 1> wrapped_args = {
                AnyWrapper(),
                AnyWrapper(std::forward<Args>(args))...
            };
            return std::any_cast<R>(callback(obj, &wrapped_args));
        }
    private:
        std::function<std::any(void *, std::any)> callback;
        std::function<AnyWrapper(std::any)> create_result;
    };
}
