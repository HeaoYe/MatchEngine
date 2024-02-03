#include <MatchEngine/function/user_interface.hpp>

namespace MatchEngine {
    PointerWrapper<LoggerSystem> logger_system { nullptr };
    PointerWrapper<InputSystem> input {nullptr};
    PointerWrapper<EventSystem> event_system { nullptr };
}
