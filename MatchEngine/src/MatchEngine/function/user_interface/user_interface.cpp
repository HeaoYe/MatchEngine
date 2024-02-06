#include <MatchEngine/function/user_interface/user_interface.hpp>

namespace MatchEngine::UserInterface {
    PointerWrapper<ReflectSystem> reflect { nullptr };
    PointerWrapper<LoggerSystem> logger_system { nullptr };
    PointerWrapper<InputSystem> input {nullptr};
    PointerWrapper<EventSystem> event_system { nullptr };
    PointerWrapper<ComponentTypeUUIDSystem> component_type_uuid_system { nullptr };
    PointerWrapper<ResourceSystem> resource_system { nullptr };
}
