#pragma once

#include <MatchEngine/core/base/macro.hpp>

namespace MatchEngine {
    class MatchEngine;
    
    // 包装指针,防止用户delete游戏引擎的运行时系统
    template<class T>
    class PointerWrapper {
        NoCopyMoveConstruction(PointerWrapper)
        friend MatchEngine;
    public:
        PointerWrapper(T *ptr) : ptr(ptr) {}
        ~PointerWrapper() { ptr = nullptr; }
        
        T *operator->() { return ptr; }
    private:
        T *ptr { nullptr };
    };
}
