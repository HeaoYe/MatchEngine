#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/function/event/event_layer.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include <map>
#include <list>

namespace MatchEngine {
    // 事件系统
    class EventSystem final : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(EventSystem)
    public:
        void attachEventLayer(float priority, const std::string &name);
        void detachEventLayer(const std::string &name);

        template <class Event>
        ListenerUUID<Event> addEventListener(ListenerCallback<Event> listener, const std::string &layer_name = "default") {
            if (auto iter = layers_map.find(layer_name); iter != layers_map.end()) {
                return iter->second->addEventListener<Event>(listener);
            } else {
                MCH_CORE_ERROR("EventLayer {} has not been attached.", layer_name);
                return ListenerUUID<Event>(INVALID_UUID);
            }
        }

        template <class Event>
        void removeEventListener(ListenerUUID<Event> uuid) {
            if (uuid.asUUIDType() == INVALID_UUID) {
                MCH_CORE_ERROR("Unavailable ListenerUUID.");
                return;
            }
            bool found = false;
            for (auto &[layer_name, listeners] : Event::listeners) {
                if (listeners.find(uuid.asUUIDType()) != listeners.end()) {
                    listeners.erase(uuid.asUUIDType());
                    found = true;
                    break;
                }
            }
            if (!found) {
                MCH_CORE_ERROR("EventListener with uuid {} has been removed.", uuid.asUUIDType())
            }
        }
    private:
        template <class Event>
        void dispatch(Event event) {
            for (auto *layer : layers) {
                bool is_handled = layer->dispatch<Event>(event);
                if (is_handled) {
                    return;
                }
            }
        }
        void registerEventCallbacks();
    private:
        std::map<std::string, EventLayer *> layers_map;
        std::list<EventLayer *> layers;
    };
}
