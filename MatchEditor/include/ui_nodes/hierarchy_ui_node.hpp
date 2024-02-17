#pragma once

#include <ui.hpp>

namespace MatchEditor {
    // 场景GameObject查看器
    class HierarchyUINode : public UINode {
    public:
        void onLoadScene() override;
        void onUnloadScene() override;
        void render() override;
    };
}
