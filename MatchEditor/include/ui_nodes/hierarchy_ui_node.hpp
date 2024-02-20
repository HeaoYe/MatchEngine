#pragma once

#include <ui.hpp>

namespace MatchEditor {
    // 场景GameObject查看器
    class HierarchyUINode : public UINode {
    public:
        void selectGameObject(MatchEngine::Game::GameObjectUUID uuid = MatchEngine::Game::GameObjectUUID(-1));

        void onLoadScene() override;
        void onUnloadScene() override;
        void render() override;
    private:
        std::map<MatchEngine::Game::GameObjectUUID, bool> ui_is_selected;
    };
}
