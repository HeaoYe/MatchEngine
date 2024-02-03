#pragma once

#include <MatchEngine/function/event/listener.hpp>
#include <cstdint>
#include <string>
#include <map>

namespace MatchEngine {
    #define DeclareEvent(category, type, ...) \
    struct type##Event { \
        __VA_ARGS__ \
        static EventCategory GetCategory() { \
            return EventCategory::e##category; \
        } \
        EventCategory getCategory() const { \
            return EventCategory::e##category; \
        } \
        static std::string GetCategoryName() { \
            return std::string(#category); \
        } \
        std::string getCategoryName() const { \
            return std::string(#category); \
        } \
        static EventType GetType() { \
            return EventType::e##type; \
        } \
        EventType getType() const { \
            return EventType::e##type; \
        } \
        static std::string GetTypeName() { \
            return std::string(#type); \
        } \
        std::string getTypeName() const { \
            return std::string(#type); \
        } \
        static ::std::map<::std::string, ::std::map<ListenerUUIDType, ListenerCallback<type##Event>>> listeners; \
        static ::MatchEngine::ListenerUUID<type##Event> current_uuid; \
    };

    #define ImplementEvent(type) \
    ::std::map<::std::string, std::map<ListenerUUIDType, ListenerCallback<type##Event>>> type##Event::listeners; \
    ::MatchEngine::ListenerUUID<type##Event> type##Event::current_uuid;

    enum class EventCategory {
        eWindow,
        eKey,
        eInput,
        eMouse,
    };

    enum class EventType {
        eWindowResized,
        eWindowMoved,
        eWindowClosed,
        eWindowFocusd,
        eWindowLostFocus,
        eKeyPressed,
        eKeyReleased,
        eInputChar,
        eMousePressed,
        eMouseReleased,
        eMouseMoved,
        eMouseScroll,
    };

    enum class MouseButton : size_t {
        eLeft = 0,
        eMiddle,
        eRight,
        eUnknown
    };

    enum class Key : size_t {
        eTab = 0,
        eLeftArrow,
        eRightArrow,
        eUpArrow,
        eDownArrow,
        ePageUp,
        ePageDown,
        eHome,
        eEnd,
        eInsert,
        eDelete,
        eBackspace,
        eSpace,
        eEnter,
        eEscape,
        eLeftCtrl, eLeftShift, eLeftAlt, eLeftSuper,
        eRightCtrl, eRightShift, eRightAlt, eRightSuper,
        eMenu,
        e0, e1, e2, e3, e4, e5, e6, e7, e8, e9,
        eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ,
        eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT,
        eU, eV, eW, eX, eY, eZ,
        eF1, eF2, eF3, eF4, eF5, eF6,
        eF7, eF8, eF9, eF10,eF11, eF12,
        eApostrophe,    // '
        eComma,         // ,
        eMinus,         // -
        eDot,           // .
        eSlash,         // /
        eSemicolon,     // ;
        eEqual,         // =
        eLeftBracket,   // [
        eRightBracket,  // ]
        eBackslash,     // '\'
        eGraveAccent,   // `
        eCapsLock,
        eScrollLock,
        eNumLock,
        ePrintScreen,
        ePause,
        eUnknown
    };

    // 全部事件
    DeclareEvent(Window, WindowResized, int32_t width, height;)
    DeclareEvent(Window, WindowMoved, int32_t x, y;)
    DeclareEvent(Window, WindowClosed)
    DeclareEvent(Window, WindowFocusd)
    DeclareEvent(Window, WindowLostFocus)
    DeclareEvent(Key, KeyPressed, Key key;)
    DeclareEvent(Key, KeyReleased, Key key;)
    DeclareEvent(Input, InputChar, uint32_t data;)
    DeclareEvent(Mouse, MousePressed, MouseButton button;)
    DeclareEvent(Mouse, MouseReleased, MouseButton button;)
    DeclareEvent(Mouse, MouseMoved, double x, y;)
    DeclareEvent(Mouse, MouseScroll, double delta_x, delta_y;)
}
