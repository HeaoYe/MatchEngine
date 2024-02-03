#include <MatchEngine/function/input/event_system.hpp>
#include <Match/core/window.hpp>
#include "internal.hpp"

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
        registerEventCallbacks();

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

    static MouseButton convertGLFWMouseButton(int button);
    static Key convertGLFWKey(int key);

    void EventSystem::registerEventCallbacks() {
        Match::window->poll_events();
        
        auto *native_pointer = Match::window->get_glfw_window();
        glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int width, int height){
            global_runtime_context->event_system->dispatch<WindowResizedEvent>({ .width = width, .height = height });
        });

        glfwSetWindowPosCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int x, int y) {
            global_runtime_context->event_system->dispatch<WindowMovedEvent>({ .x = x, .y = y });
        });

        glfwSetWindowCloseCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *) {
            global_runtime_context->event_system->dispatch<WindowClosedEvent>({});
        });

        glfwSetWindowFocusCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int focused) {
            switch (focused) {
            case GLFW_TRUE:
                global_runtime_context->event_system->dispatch<WindowFocusdEvent>({});
                break;
            case GLFW_FALSE:
                global_runtime_context->event_system->dispatch<WindowLostFocusEvent>({});
                break;
            default:
                break;
            }
        });

        glfwSetKeyCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int keycode, int scancode, int action, int mods) {
            auto key = convertGLFWKey(keycode);
            if (key == Key::eUnknown) {
                return;
            }
            switch (action) {
            case GLFW_PRESS:
                global_runtime_context->input_system->currentState()->key_states[static_cast<size_t>(key)] = InputSystem::ButtonState::ePressed;
                global_runtime_context->event_system->dispatch<KeyPressedEvent>({ .key = key });
                break;
            case GLFW_RELEASE:
                global_runtime_context->input_system->currentState()->key_states[static_cast<size_t>(key)] = InputSystem::ButtonState::eReleased;
                global_runtime_context->event_system->dispatch<KeyReleasedEvent>({ .key = key });
                break;
            default:
                break;
            }
        });

        glfwSetCharCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, unsigned int codepoint) {
            global_runtime_context->event_system->dispatch<InputCharEvent>({ .data = codepoint });
        });

        glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int button, int action, int mods) {
            auto btn = convertGLFWMouseButton(button);
            if (btn == MouseButton::eUnknown)
                return;
            switch (action) {
            case GLFW_PRESS:
                global_runtime_context->input_system->currentState()->mouse_button_states[static_cast<size_t>(btn)] = InputSystem::ButtonState::ePressed;
                global_runtime_context->event_system->dispatch<MousePressedEvent>({ .button = btn });
                break;
            case GLFW_RELEASE:
                global_runtime_context->input_system->currentState()->mouse_button_states[static_cast<size_t>(btn)] = InputSystem::ButtonState::eReleased;
                global_runtime_context->event_system->dispatch<MouseReleasedEvent>({ .button = btn });
                break;
            default:
                break;
            }
        });

        glfwSetCursorPosCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, double xpos, double ypos) {
            global_runtime_context->input_system->currentState()->mouse_pos = { xpos, ypos };
            global_runtime_context->input_system->currentState()->mouse_delta = global_runtime_context->input_system->currentState()->mouse_pos - global_runtime_context->input_system->lastState()->mouse_pos;
            global_runtime_context->event_system->dispatch<MouseMovedEvent>({ .x = xpos, .y = ypos });
        });

        glfwSetScrollCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, double xoffset, double yoffset) {
            global_runtime_context->input_system->currentState()->mouse_scroll = { xoffset, yoffset };
            global_runtime_context->event_system->dispatch<MouseScrollEvent>({ .delta_x = xoffset, .delta_y = yoffset });
        });
    }

    MouseButton convertGLFWMouseButton(int button) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            return MouseButton::eLeft;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return MouseButton::eMiddle;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return MouseButton::eRight;
        default:
            return MouseButton::eUnknown;
        }
    }

    #define caseKey(glfw_key_name, key_name) \
    case GLFW_KEY_##glfw_key_name: \
        return Key::e##key_name;
    Key convertGLFWKey(int key) {
        switch (key) {
        caseKey(TAB, Tab)
        caseKey(LEFT, LeftArrow)
        caseKey(RIGHT, RightArrow)
        caseKey(UP, UpArrow)
        caseKey(DOWN, DownArrow)
        caseKey(PAGE_UP, PageUp)
        caseKey(PAGE_DOWN, PageDown)
        caseKey(HOME, Home)
        caseKey(END, End)
        caseKey(INSERT, Insert)
        caseKey(DELETE, Delete)
        caseKey(BACKSPACE, Backspace)
        caseKey(SPACE, Space)
        caseKey(ENTER, Enter)
        caseKey(ESCAPE, Escape)
        caseKey(LEFT_CONTROL, LeftCtrl)
        caseKey(LEFT_SHIFT, LeftShift)
        caseKey(LEFT_ALT, LeftAlt)
        caseKey(LEFT_SUPER, LeftSuper)
        caseKey(RIGHT_CONTROL, RightCtrl)
        caseKey(RIGHT_SHIFT, RightShift)
        caseKey(RIGHT_ALT, RightAlt)
        caseKey(RIGHT_SUPER, RightSuper)
        caseKey(MENU, Menu)
        caseKey(0, 0)
        caseKey(1, 1)
        caseKey(2, 2)
        caseKey(3, 3)
        caseKey(4, 4)
        caseKey(5, 5)
        caseKey(6, 6)
        caseKey(7, 7)
        caseKey(8, 8)
        caseKey(9, 9)
        caseKey(A, A)
        caseKey(B, B)
        caseKey(C, C)
        caseKey(D, D)
        caseKey(E, E)
        caseKey(F, F)
        caseKey(G, G)
        caseKey(H, H)
        caseKey(I, I)
        caseKey(J, J)
        caseKey(K, K)
        caseKey(L, L)
        caseKey(M, M)
        caseKey(N, N)
        caseKey(O, O)
        caseKey(P, P)
        caseKey(Q, Q)
        caseKey(R, R)
        caseKey(S, S)
        caseKey(T, T)
        caseKey(U, U)
        caseKey(V, V)
        caseKey(W, W)
        caseKey(X, X)
        caseKey(Y, Y)
        caseKey(Z, Z)
        caseKey(F1, F1)
        caseKey(F2, F2)
        caseKey(F3, F3)
        caseKey(F4, F4)
        caseKey(F5, F5)
        caseKey(F6, F6)
        caseKey(F7, F7)
        caseKey(F8, F8)
        caseKey(F9, F9)
        caseKey(F10, F10)
        caseKey(F11, F11)
        caseKey(F12, F12)
        caseKey(APOSTROPHE, Apostrophe)
        caseKey(COMMA, Comma)
        caseKey(MINUS, Minus)
        caseKey(PERIOD, Dot)
        caseKey(SLASH, Slash)
        caseKey(SEMICOLON, Semicolon)
        caseKey(EQUAL, Equal)
        caseKey(LEFT_BRACKET, LeftBracket)
        caseKey(RIGHT_BRACKET, RightBracket)
        caseKey(BACKSLASH, Backslash)
        caseKey(GRAVE_ACCENT, GraveAccent)
        caseKey(CAPS_LOCK, CapsLock)
        caseKey(SCROLL_LOCK, ScrollLock)
        caseKey(NUM_LOCK, NumLock)
        caseKey(PRINT_SCREEN, PrintScreen)
        caseKey(PAUSE, Pause)
        default:
            return Key::eUnknown;
        }
    }
    #undef case_key
}
