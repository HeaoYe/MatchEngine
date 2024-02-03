#pragma once

#include <MatchEngine/function/event/listener.hpp>
#include <MatchEngine/core/base/macro.hpp>
#include <string>

namespace MatchEngine {
    class EventLayer {
        NoCopyMoveConstruction(EventLayer)
    public:
        EventLayer(float priority, const std::string &name);
        ~EventLayer();

        template <class Event>
        ListenerUUID<Event> addEventListener(ListenerCallback<Event> listener) {
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
}
