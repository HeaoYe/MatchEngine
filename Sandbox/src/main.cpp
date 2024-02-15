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

    // 设置资产的根目录
    assets_system->setRootDir("Sandbox/resource");
    // 加载模型
    auto dragon_mesh_id = assets_system->loadMesh("dragon_lods.obj");

    // 创建相机
    auto camera = scene->createGameObject("相机");

    // 创建8*8*8条龙
    int n = 8, n2 = n / 2;
    for (int i = 0; i < n * n * n; i ++) {
        auto dragon = scene->createGameObject("龙");
        auto t = new MatchEngine::Game::TransformComponent();
        t->location = { i % n - n2, (i / n) % n - n2, ((i / n) / n) % n - n2 };
        dragon->addComponet(t);
        dragon->addComponet(new MatchEngine::Game::MeshComponent(dragon_mesh_id));
    }

    // 为相机添加组件
    camera->addComponet(new MatchEngine::Game::CameraControllerComponent());
    camera->addComponet(new MatchEngine::Game::TransformComponent());
    camera->addComponet(new MatchEngine::Game::CameraComponent());

    // 引擎运行入口
    engine.gameLoop();

    // 引擎销毁入口
    engine.destroy();
    return 0;
}
