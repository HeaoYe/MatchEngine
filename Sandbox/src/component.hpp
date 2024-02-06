#pragma once

#include <MatchEngine/MatchEngine.hpp>

// 自定义组件

REFLECT_CLASS(NAME = "Pen")
class Pen : public MatchEngine::Game::Component {
    DECLARE_COMPONENT(Pen)
    ALLOW_REFLECT_PRIVATE()
private:
    REFLECT_FUNCTION()
    void write() {
        MCH_CLIENT_INFO("{} 用笔写字", master->getName())
    }
};

// 声明反射信息(必须声明, 且NAME = "类名")
REFLECT_CLASS(NAME = PersonController)
// 人物控制器组件
class PersonController : public MatchEngine::Game::Component {
    // 声明组件信息
    DECLARE_COMPONENT(PersonController)

    void onStart() override {
        // master  拥有该Component的GameObject对象
        // 强制获取 TransformComponent, 如果GameObject对象没有TransformComponent
        // master->forceGetComponent<MatchEngine::Game::TransformComponent>();
        // 查询 TransformComponent, 如果没有返回nullptr
        // master->queryComponent<MatchEngine::Game::TransformComponent>();
        // 也可以通过类名获取Component
        // master->forceGetComponent("TransformComponent");
        // master->queryComponent("TransformComponent");

        // 获取TransformComponent
        auto transform = master->forceGetComponent<MatchEngine::Game::TransformComponent>();
        // 获取location成员变量的地址
        location = &transform->location;

        // 查询Pen
        pen = master->queryComponent<Pen>();
    }

    void onTick(float dt) override {
        // 控制人的移动
        using namespace MatchEngine::UserInterface;
        using MatchEngine::Key;
        if (input->isKeyPressed(Key::eW)) {
            location->z += 0.1;
        }
        if (input->isKeyPressed(Key::eS)) {
            location->z -= 0.1;
        }
        if (input->isKeyPressed(Key::eD)) {
            location->x += 0.1;
        }
        if (input->isKeyPressed(Key::eA)) {
            location->x -= 0.1;
        }
        if (input->isKeyPressed(Key::eSpace)) {
            location->y += 0.1;
        }
        if (input->isKeyPressed(Key::eLeftShift)) {
            location->y -= 0.1;
        }
        // 输出位置信息
        MCH_CLIENT_INFO("{} 在 {} {} {}", master->getName(), location->x, location->y, location->z)
        if (pen != nullptr) {
            // 通过反射调用write函数
            pen->invoke("write");
        } else {
            MCH_CLIENT_INFO("{} 啥也没做", master->getName());
        }
        if (location->y > 10000) {
            MCH_CLIENT_WARN("更换场景")
            scene_manager->loadScene("My Second Scene");
        }
    }
private:
    glm::vec3 *location;
    Pen *pen;
};
