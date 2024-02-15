#include <editor.hpp>
#include <MatchEngine/MatchEngine.hpp>
#include <random>

int main() {
    // 通过4行代码，完成编辑游戏引擎
    using namespace MatchEngine::UserInterface;

    // 创建并初始化游戏引擎
    auto engine = new MatchEngine::MatchEngine();
    // 在游戏引擎初始化后，创建游戏引擎编辑器
    auto editor = new MatchEditor::MatchEditor(engine);
    engine->initialize();

    // 创建场景
    auto scene = scene_manager->createScene("My First Scene");
    
    // 添加相机
    auto camera = scene->createGameObject("相机");
    camera->addComponet(new MatchEngine::Game::TransformComponent());
    camera->addComponet(new MatchEngine::Game::PerspectiveCameraComponent(60, 1920 / 1080.0f, 0.1, 1000));
    camera->addComponet(new MatchEngine::Game::CameraControllerComponent());

    // 剔除了60% ~ 80%
    std::random_device device;
    std::mt19937 mt(device());
    std::uniform_real_distribution<float> random_scale(0.7, 1.8);
    // 加载龙
    assets_system->setRootDir("Sandbox/resource");
    // auto dragon_mesh_id = assets_system->loadMesh("dragon_lods.obj", { "dragon_LOD5" });
    auto dragon_mesh_id = assets_system->loadMesh("dragon_lods.obj");
    int n = 24, n2 = n / 2;
    for (int i = 0; i < n * n * n; i ++) {
        auto dragon = scene->createGameObject("龙_" + std::to_string(i));
        auto t = new MatchEngine::Game::TransformComponent();
        t->location = { i % n - n2, (i / n) % n - n2, ((i / n) / n) % n - n2 };
        t->scale = { random_scale(mt), random_scale(mt), random_scale(mt) };
        dragon->addComponet(t);
        dragon->addComponet(new MatchEngine::Game::MeshComponent(dragon_mesh_id));
    }
    
    // 在游戏引擎主循环前，初始化游戏引擎编辑器
    editor->initialize();

    // 游戏主循环
    engine->gameLoop();

    // 在游戏引擎销毁前，销毁游戏引擎编辑器
    editor->destroy();
    delete editor;

    // 销毁游戏引擎
    engine->destroy();
    delete engine;
    return 0;
}
