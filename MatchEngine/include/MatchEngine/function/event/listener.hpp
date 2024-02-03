#pragma once

#include <functional>

namespace MatchEngine {
    template <class Event>
    using ListenerCallback = std::function<bool(Event &)>;

    using ListenerUUIDType = size_t;
    constexpr ListenerUUIDType INVALID_UUID = ListenerUUIDType(-1);
    
    extern ListenerUUIDType global_current_uuid;;

    template <class Event>
    class ListenerUUID {
    public:
        ListenerUUID() : uuid(0) {}

        ListenerUUID(ListenerUUIDType uuid) : uuid(uuid) {}

        ListenerUUIDType asUUIDType() {
            return uuid;
        }
    private:
        ListenerUUIDType uuid;
    };
}
