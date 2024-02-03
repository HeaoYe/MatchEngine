#include <MatchEngine/function/input/input_system.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include <memory.h>

namespace MatchEngine {
    InputSystem::InputSystem() {
        initializeRuntimeSystem();

        memset(input_states, 0, sizeof(input_states));
        current_state_index = 0;

        state = RuntimeSystem::State::eInitialized;
    }

    InputSystem::~InputSystem() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;
    }

    void InputSystem::swapState() {
        current_state_index = !current_state_index;
    }

    InputSystem::InputState *InputSystem::currentState() {
        return &input_states[current_state_index];
    }

    InputSystem::InputState *InputSystem::lastState() {
        return &input_states[!current_state_index];
    }

    bool InputSystem::isKeyPressed(Key key) {
        return input_states[current_state_index].key_states[static_cast<size_t>(key)] == ButtonState::ePressed;
    }

    bool InputSystem::wasKeyPressed(Key key) {
        return input_states[!current_state_index].key_states[static_cast<size_t>(key)] == ButtonState::ePressed;
    }
    
    bool InputSystem::isKeyReleased(Key key) {
        return input_states[current_state_index].key_states[static_cast<size_t>(key)] == ButtonState::eReleased;
    }

    bool InputSystem::wasKeyReleased(Key key) {
        return input_states[!current_state_index].key_states[static_cast<size_t>(key)] == ButtonState::eReleased;
    }

    bool InputSystem::isMouseButtonPressed(MouseButton button) {
        return input_states[current_state_index].mouse_button_states[static_cast<size_t>(button)] == ButtonState::ePressed;
    }

    bool InputSystem::wasMouseButtonPressed(MouseButton button) {
        return input_states[!current_state_index].mouse_button_states[static_cast<size_t>(button)] == ButtonState::ePressed;
    }

    bool InputSystem::isMouseButtonReleased(MouseButton button) {
        return input_states[current_state_index].mouse_button_states[static_cast<size_t>(button)] == ButtonState::eReleased;
    }

    bool InputSystem::wasMouseButtonReleased(MouseButton button) {
        return input_states[!current_state_index].mouse_button_states[static_cast<size_t>(button)] == ButtonState::eReleased;
    }

    glm::dvec2 InputSystem::getMousePos() {
        return input_states[current_state_index].mouse_pos;
    }

    glm::dvec2 InputSystem::getLastMousePos() {
        return input_states[!current_state_index].mouse_pos;
    }

    glm::dvec2 InputSystem::getMouseDelta() {
        return input_states[current_state_index].mouse_delta;
    }

    glm::dvec2 InputSystem::getLastMouseDelta() {
        return input_states[!current_state_index].mouse_delta;
    }

    glm::dvec2 InputSystem::getMouseScroll() {
        return input_states[current_state_index].mouse_scroll;
    }

    glm::dvec2 InputSystem::getLastMouseScroll() {
        return input_states[!current_state_index].mouse_scroll;
    }

    double InputSystem::getMousePosX() {
        return input_states[current_state_index].mouse_pos.x;
    }

    double InputSystem::getLastMousePosX() {
        return input_states[!current_state_index].mouse_pos.x;
    }

    double InputSystem::getMousePosY() {
        return input_states[current_state_index].mouse_pos.y;
    }

    double InputSystem::getLastMousePosY() {
        return input_states[!current_state_index].mouse_pos.y;
    }

    double InputSystem::getMouseDeltaX() {
        return input_states[current_state_index].mouse_delta.x;
    }

    double InputSystem::getLastMouseDeltaX() {
        return input_states[!current_state_index].mouse_delta.x;
    }

    double InputSystem::getMouseDeltaY() {
        return input_states[current_state_index].mouse_delta.y;
    }

    double InputSystem::getLastMouseDeltaY() {
        return input_states[!current_state_index].mouse_delta.y;
    }

    double InputSystem::getMouseScrollX() {
        return input_states[current_state_index].mouse_scroll.x;
    }

    double InputSystem::getLastMouseScrollX() {
        return input_states[!current_state_index].mouse_scroll.x;
    }

    double InputSystem::getMouseScrollY() {
        return input_states[current_state_index].mouse_scroll.y;
    }

    double InputSystem::getLastMouseScrollY() {
        return input_states[!current_state_index].mouse_scroll.y;
    }
}
