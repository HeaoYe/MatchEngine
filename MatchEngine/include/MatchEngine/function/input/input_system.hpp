#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/function/event/event.hpp>
#include <glm/vec2.hpp>

namespace MatchEngine {
    // 输入系统
    class InputSystem final : public RuntimeSystem {
        friend class MatchEngine;
        friend class EventSystem;
        DECLARE_RUNTIME_SYSTEM(InputSystem)
    private:
        enum class ButtonState : uint8_t {
            eReleased = 0,
            ePressed,
        };

        struct InputState {
            ButtonState key_states[static_cast<size_t>(Key::eUnknown)];
            ButtonState mouse_button_states[static_cast<size_t>(MouseButton::eUnknown)];
            glm::dvec2 mouse_pos;
            glm::dvec2 mouse_delta;
            glm::dvec2 mouse_scroll;
        };
    public:
        bool isKeyPressed(Key key);
        bool wasKeyPressed(Key key);
        bool isKeyReleased(Key key);
        bool wasKeyReleased(Key key);

        bool isMouseButtonPressed(MouseButton button);
        bool wasMouseButtonPressed(MouseButton button);
        bool isMouseButtonReleased(MouseButton button);
        bool wasMouseButtonReleased(MouseButton button);

        glm::dvec2 getMousePos();
        glm::dvec2 getLastMousePos();
        glm::dvec2 getMouseDelta();
        glm::dvec2 getLastMouseDelta();
        glm::dvec2 getMouseScroll();
        glm::dvec2 getLastMouseScroll();

        double getMousePosX();
        double getLastMousePosX();
        double getMousePosY();
        double getLastMousePosY();
        double getMouseDeltaX();
        double getLastMouseDeltaX();
        double getMouseDeltaY();
        double getLastMouseDeltaY();
        double getMouseScrollX();
        double getLastMouseScrollX();
        double getMouseScrollY();
        double getLastMouseScrollY();
    private:
        void swapState();
        InputState *currentState();
        InputState *lastState();
        void registerEventListener();
    private:
        InputState input_states[2] {};
        size_t current_state_index;
    };
}
