#pragma once

#include <MatchEngine/core/logger/logger.hpp>
#include <MatchEngine/function/input/event_system.hpp>

namespace MatchEngine {
    // 用户接口

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

    // 提供给用户使用的运行时系统
    extern PointerWrapper<LoggerSystem> logger_system;
    extern PointerWrapper<EventSystem> event_system;
}
