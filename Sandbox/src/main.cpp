#include <MatchEngine/MatchEngine.hpp>
#include <iostream>

int main() {
    MatchEngine::MatchEngine engine;

    // 引擎初始化入口
    if (!engine.initialize()) {
        std::cout << "Failed initialize engine." << std::endl;
        return -1;
    }

    // MatchEngine通过EventLayer管理事件监听器
    // 事件会根据EventLayer的优先级 依次 分发给各个EventLayer
    // 默认有一个名为defaut的EventLayer, 他的优先级为0
    // 优先级可以为负数
    MatchEngine::event_system->attachEventLayer(
        1.0,
        "My Event Layer High Priority"
    );
    MatchEngine::event_system->attachEventLayer(
        -1.0,
        "My Event Layer Low Priority"
    );

    // 向EventLayer添加事件监听器, 默认添加到名为defaut的EventLayer
    // addEventListener会返回一个事件监听器的UUID,可以使用这个UUID移除对应的事件监听器
    // 事件监听器返回布尔值,代表该事件是否已被处理, 若已被处理,则该事件不会被传递后续事件监听器以及EventLayer
    MatchEngine::event_system->addEventListener<MatchEngine::MouseMovedEvent>([](MatchEngine::MouseMovedEvent &event) {
        MCH_CLIENT_INFO("Default EventLayer: {}: {} {}", event.getTypeName(), event.x, event.y)
        return false;
    });
    auto uuid = MatchEngine::event_system->addEventListener<MatchEngine::MouseMovedEvent>([](MatchEngine::MouseMovedEvent &event) {
        MCH_CLIENT_INFO("High Priority EventLayer: {}: {} {}", event.getTypeName(), event.x, event.y)
        return true;
    }, "My Event Layer High Priority");
    MatchEngine::event_system->addEventListener<MatchEngine::MouseMovedEvent>([](MatchEngine::MouseMovedEvent &event) {
        MCH_CLIENT_INFO("Low Priority EventLayer: {}: {} {}", event.getTypeName(), event.x, event.y)
        return false;
    }, "My Event Layer Low Priority");

    // 分发事件, 未来会由窗口系统自动分发
    MatchEngine::event_system->dispatch(MatchEngine::MouseMovedEvent { 10, 200 });

    // 移除事件监听器
    MatchEngine::event_system->removeEventListener(uuid);

    // 也可以直接移除整个EventLayer
    MatchEngine::event_system->detachEventLayer("default");

    // 再次分发事件
    MatchEngine::event_system->dispatch(MatchEngine::MouseMovedEvent { 9999999, 88888 });

    // 引擎运行入口
    // engine.run();

    // 引擎销毁入口
    engine.destroy();
    return 0;
}
