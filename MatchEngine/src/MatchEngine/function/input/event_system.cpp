#include <MatchEngine/function/input/event_system.hpp>

namespace MatchEngine {
    ListenerUUIDType global_current_uuid = 0;

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

    EventSystem::EventSystem() {
        initializeRuntimeSystem();

        attachEventLayer(0, "default");
        state = RuntimeSystem::State::eInitialized;
    }

    EventSystem::~EventSystem() {
        destoryRuntimeSystem();
        
        state = RuntimeSystem::State::eExited;
        layers_map.clear();
        for (auto layer : layers) {
            delete layer;
        }
        layers.clear();
    }

    void EventSystem::attachEventLayer(float priority, const std::string &name) {
        if (layers_map.find(name) != layers_map.end()) {
            MCH_CORE_ERROR("EventLayer {} has been attached.", name);
            return;
        }
        auto *layer_ptr = new EventLayer(priority, name);
        auto position = layers.begin();
        for(;position != layers.end(); position ++) {
            if (layer_ptr->getPriority() > (*position)->getPriority()) {
                break;
            }
        }
        layers.insert(position, layer_ptr);
        layers_map[name] = layer_ptr;
    }

    void EventSystem::detachEventLayer(const std::string &name) {
        if (auto iter = layers_map.find(name); iter != layers_map.end()) {
            auto position = layers.begin();
            for(;position != layers.end(); position ++) {
                if ((*position) == iter->second) {
                    break;
                }
            }
            assert(position != layers.end());
            layers.erase(position);
            layers_map.erase(name);
            delete iter->second;
        } else {
            MCH_CORE_ERROR("EventLayer {} has not been attached.", name);
        }
    }
}
