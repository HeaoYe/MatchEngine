#include <MatchEngine/MatchEngine.hpp>
#include <iostream>

int main() {
    MatchEngine::MatchEngine engine;

    // 引擎初始化入口
    if (!engine.initialize()) {
        std::cout << "Failed initialize engine." << std::endl;
        return -1;
    }

    MatchEngine::event_system->addEventListener<MatchEngine::MouseMovedEvent>([](MatchEngine::MouseMovedEvent &event) {
        // MCH_CLIENT_INFO("{}: {} {}", event.getTypeName(), event.x, event.y)
        return false;
    });

    MatchEngine::event_system->addEventListener<MatchEngine::KeyPressedEvent>([](MatchEngine::KeyPressedEvent &event) {
        if (event.key == MatchEngine::Key::eEscape) {
            MCH_CLIENT_INFO("按下了ESC")
            MCH_CLIENT_INFO("按下时鼠标位置为 {}, {}", MatchEngine::input->getMousePosX(), MatchEngine::input->getMousePosY())
        }
        return false;
    });

    MatchEngine::event_system->addEventListener<MatchEngine::KeyReleasedEvent>([](MatchEngine::KeyReleasedEvent &event) {
        if (event.key == MatchEngine::Key::eEscape) {
            MCH_CLIENT_INFO("释放时鼠标 上一帧位置为 {}, {}", MatchEngine::input->getLastMousePosX(), MatchEngine::input->getLastMousePosY())
            MCH_CLIENT_INFO("释放了ESC")
        }
        return false;
    });

    // 引擎运行入口
    engine.run();

    // 引擎销毁入口
    engine.destroy();
    return 0;
}
