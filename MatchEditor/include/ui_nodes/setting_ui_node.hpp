#pragma once

#include <ui.hpp>

namespace MatchEditor {
    // 设置UI节点，显示帧率，可见实体数
    class SettingUINode : public UINode {
    public:
        void render() override;
    };
}
