#pragma once

#include <MatchEngine/MatchEngine.hpp>

// 自定义组件
REFLECT_CLASS(NAME = "Pen")
class Pen : public MatchEngine::Game::Component {
    DECLARE_COMPONENT(Pen)
    ALLOW_REFLECT_PRIVATE()
public:
    void onCreate() override {}
    void onDestroy() override {}
    void onClone(MatchEngine::Game::Component *) const override {}
    void onTick(float dt) override {}
private:
    REFLECT_FUNCTION()
    void write(glm::vec3 pos) {
        MCH_CLIENT_INFO("在 {} {} {} 用笔写字 ...........", pos.x, pos.y, pos.z)
    }
};

REFLECT_CLASS(NAME = "PersonController")
class PersonController : public MatchEngine::Game::Component {
    DECLARE_COMPONENT(PersonController)
public:
    void onCreate() override {
        // master  是指向拥有当前组件实例的GameObject的指针
        // master->getComponents() 获取GameObject的所有组件
        // master->findComponent() 查找一个组件

        // 获取GameObject的Location组件
        auto loc_comp = master->findComponent<MatchEngine::Game::LocationComponent>();

        // 获取组件的数据
        auto &location = loc_comp->getValueMutReference<glm::vec3>("location");
        location_ptr = &location;

        pen = master->findComponent<Pen>();
    }
    void onDestroy() override {}
    void onClone(MatchEngine::Game::Component *) const override {}
    void onTick(float dt) override {
        // MCH_CLIENT_INFO("TICK !!!!!!!!!!!!!")
        // 控制location
        if (MatchEngine::UserInterface::input->isKeyPressed(MatchEngine::Key::eD)) {
            location_ptr->x += 0.1;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(MatchEngine::Key::eA)) {
            location_ptr->x -= 0.1;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(MatchEngine::Key::eW)) {
            location_ptr->y += 0.1;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(MatchEngine::Key::eS)) {
            location_ptr->y -= 0.1;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(MatchEngine::Key::eLeftShift)) {
            location_ptr->z -= 0.1;
        }
        if (MatchEngine::UserInterface::input->isKeyPressed(MatchEngine::Key::eSpace)) {
            location_ptr->z += 0.1;
        }
        // MCH_CLIENT_INFO("Location: {} {} {}", location_ptr->x, location_ptr->y, location_ptr->z)

        // 通过反射调用Pen组件的write函数
        pen->invoke("write", *location_ptr);
    }
private:
    glm::vec3 *location_ptr;
    Pen *pen;
};
