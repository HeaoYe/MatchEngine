#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <utility>

namespace MatchEngine {

    // 单例类
    template <class T>
    class Singleton {
        NoCopyMoveConstruction(Singleton)
    public:
        Singleton() {
            instance = nullptr;
        }

        ~Singleton() {
            destory();
        }

        template <typename... Args>
        void initialize(Args &&... args) {
            instance = new T(std::forward<Args>(args)...);
        }

        void destory() {
            if (instance != nullptr) {
                delete instance;
                instance = nullptr;
            }
        }

        T *getInstance() noexcept {
            return instance;
        }

        const T *getInstance() const noexcept {
            return instance;
        }

        T *operator->() noexcept {
            return instance;
        }

        const T *operator->() const noexcept {
            return instance;
        }

        T &operator()() noexcept {
            return *instance;
        }

        const T &operator()() const noexcept {
            return *instance;
        }
    private:
        T *instance = nullptr;
    };
}