#include <MatchEngine/MatchEngine.hpp>
#include <iostream>
// 引入自动生成头文件, 包含自动生成的序列化与反序列化函数
#include <MatchEngine_AutoGenerate.hpp>
#include "component.hpp"

int main() {
    // 使用 MatchEngine 提供的用户接口 命名空间
    using namespace MatchEngine::UserInterface;

    MatchEngine::MatchEngine engine;

    // 引擎初始化入口
    if (!engine.initialize()) {
        std::cout << "Failed initialize engine." << std::endl;
        return -1;
    }

    // 创建一个场景
    auto scene = scene_manager->createScene("My First Scene");

    // 创建一个GameObject
    auto person1 = scene->createGameObject("小明");
    auto person2 = scene->createGameObject("小红");

    // 为GameObject添加组件
    person1->addComponet(new MatchEngine::Game::TransformComponent);
    person1->addComponet(new PersonController());

    person2->addComponet(new MatchEngine::Game::TransformComponent);
    person2->addComponet(new PersonController());
    person2->addComponet(new Pen());

    // 创建第二个场景
    auto scene2 = scene_manager->createScene("My Second Scene");
    auto person3 = scene2->createGameObject("无名氏");
    person3->addComponet(new MatchEngine::Game::TransformComponent);
    person3->addComponet(new PersonController());

    // 引擎运行入口
    engine.run();

    // 引擎销毁入口
    engine.destroy();
    return 0;
}
