#pragma once

#include <MatchEngine/function/input/event.hpp>
#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include <map>
#include <list>

namespace MatchEngine {
    constexpr int MAX_EVENT_LAYER_COUNT = 16;
    constexpr ListenerUUIDType INVALID_UUID = ListenerUUIDType(-1);
    extern ListenerUUIDType global_current_uuid;;

    class EventLayer {
        NoCopyMoveConstruction(EventLayer)
    public:
        EventLayer(float priority, const std::string &name);
        ~EventLayer();

        template <class Event>
        ListenerUUID<Event> addEventListener(EventListener<Event> listener) {
            Event::listeners[name].insert(std::make_pair(global_current_uuid, listener));
            global_current_uuid ++;
            return ListenerUUID<Event>(global_current_uuid - 1);
        }
        
        template <class Event>
        bool dispatch(Event &event) {
            for (auto &[uuid, listener] : Event::listeners[name]) {
                bool is_handled = listener(event);
                if (is_handled) {
                    return true;
                }
            }
            return false;
        }

        float getPriority() const { return priority; }
    private:
        float priority;
        std::string name;
    };

    // 事件系统
    class EventSystem final : public RuntimeSystem {
        NoCopyMoveConstruction(EventSystem)
    public:
        EventSystem();
        ~EventSystem() override;

        void attachEventLayer(float priority, const std::string &name);
        void detachEventLayer(const std::string &name);

        template <class Event>
        ListenerUUID<Event> addEventListener(EventListener<Event> listener, const std::string &layer_name = "default") {
            if (auto iter = layers_map.find(layer_name); iter != layers_map.end()) {
                return iter->second->addEventListener<Event>(listener);
            } else {
                MCH_CORE_ERROR("EventLayer {} has not been attached.", layer_name);
                return ListenerUUID<Event>(INVALID_UUID);
            }
        }

        template <class Event>
        void removeEventListener(ListenerUUID<Event> uuid) {
            if (uuid.asUuidType() == INVALID_UUID) {
                MCH_CORE_ERROR("Unavailable ListenerUUID.");
                return;
            }
            bool found = false;
            for (auto &[layer_name, listeners] : Event::listeners) {
                if (listeners.find(uuid.asUuidType()) != listeners.end()) {
                    listeners.erase(uuid.asUuidType());
                    found = true;
                    break;
                }
            }
            if (!found) {
                MCH_CORE_ERROR("EventListener with uuid {} has been removed.", uuid.asUuidType())
            }
        }
        
        template <class Event>
        void dispatch(Event event) {
            for (auto *layer : layers) {
                bool is_handled = layer->dispatch<Event>(event);
                if (is_handled) {
                    return;
                }
            }
        }

        std::string getSystemName() const override { return "EventSystem"; }
    private:
        std::map<std::string, EventLayer *> layers_map;
        std::list<EventLayer *> layers;
    };
}
