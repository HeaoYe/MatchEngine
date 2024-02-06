#include <MatchEngine/MatchEngine.hpp>
#include <iostream>
// 引入自动生成头文件, 包含自动生成的序列化与反序列化函数
#include <MatchEngine_AutoGenerate.hpp>
#include "my_component.hpp"


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
    auto scene = resource_system->createScene("Main Scene");
    // 创建两个个Level
    auto level1 = scene->createLevel("Level 1");
    auto level2 = scene->createLevel("Level 2");
    // 设置默认加载的Level
    // 通过名字设置
    // scene->setDefaultLevel("Level 1");
    // 或者通过Level对象设置
    scene->setDefaultLevel(level1);

    // 创建一个名为Person的GameObejectResource
    auto person_res = resource_system->createGameObjectResource("Person");
    // 为Person附加 位置 组件
    person_res->attachComponentTemplate(new MatchEngine::Game::LocationComponent ());
    // 为Person附加 控制 组件
    person_res->attachComponentTemplate(new PersonController());
    person_res->attachComponentTemplate(new Pen());

    // 为level1 附加GameObjectResource
    level1->getGameObjectResourceContainer().attachGameObjectResource(person_res);

    // 创建一个Person
    auto person = level1->createGameObject("Person");
    // 添加到Level 1中
    level1->getRootGameObject().addChild(person);

    // 让引擎加载刚才创建的场景
    engine.loadScene("Main Scene");

    // 引擎运行入口
    engine.run();

    // 引擎销毁入口
    engine.destroy();
    return 0;
}
