#include <MatchEngine/function/event/event_layer.hpp>
#include <MatchEngine/function/event/event.hpp>

namespace MatchEngine {
    EventLayer::EventLayer(float priority, const std::string &name) : priority(priority), name(name) {
        WindowResizedEvent::listeners[name] = {};
        WindowMovedEvent::listeners[name] = {};
        WindowClosedEvent::listeners[name] = {};
        WindowFocusdEvent::listeners[name] = {};
        WindowLostFocusEvent::listeners[name] = {};
        KeyPressedEvent::listeners[name] = {};
        KeyReleasedEvent::listeners[name] = {};
        InputCharEvent::listeners[name] = {};
        MousePressedEvent::listeners[name] = {};
        MouseReleasedEvent::listeners[name] = {};
        MouseMovedEvent::listeners[name] = {};
        MouseScrollEvent::listeners[name] = {};
    }

    EventLayer::~EventLayer() {
        WindowResizedEvent::listeners.erase(name);
        WindowMovedEvent::listeners.erase(name);
        WindowClosedEvent::listeners.erase(name);
        WindowFocusdEvent::listeners.erase(name);
        WindowLostFocusEvent::listeners.erase(name);
        KeyPressedEvent::listeners.erase(name);
        KeyReleasedEvent::listeners.erase(name);
        InputCharEvent::listeners.erase(name);
        MousePressedEvent::listeners.erase(name);
        MouseReleasedEvent::listeners.erase(name);
        MouseMovedEvent::listeners.erase(name);
        MouseScrollEvent::listeners.erase(name);
    }
}
