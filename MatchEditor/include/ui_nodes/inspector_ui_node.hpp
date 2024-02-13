#pragma once

#include <ui.hpp>

namespace MatchEditor {
    // GameObject组件检查器
    class InspectorUINode : public UINode {
    public:
        void render() override;
    };
}
