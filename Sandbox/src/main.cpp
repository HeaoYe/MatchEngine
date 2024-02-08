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
    // 加载龙的Mesh, 未来会由AssetsManager完成
    auto dragon_mesh_id = scene_manager->loadMesh("dragon.obj");

    // 创建龙和相机
    auto dragon = scene->createGameObject("龙");
    auto camera = scene->createGameObject("相机");

    // 为龙添加组件
    dragon->addComponet(new MatchEngine::Game::TransformComponent());
    dragon->addComponet(new MatchEngine::Game::MeshComponent(dragon_mesh_id));

    // 为相机添加组件
    camera->addComponet(new CameraController());
    camera->addComponet(new MatchEngine::Game::CameraComponent());

    // 引擎运行入口
    engine.run();

    // 引擎销毁入口
    engine.destroy();
    return 0;
}
