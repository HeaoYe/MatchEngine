#pragma once

#include <ui.hpp>
#include <ui_nodes/inspector/component_ui/component_ui_manager.hpp>

namespace MatchEditor {
    // GameObject组件检查器
    class InspectorUINode : public UINode {
    public:
        InspectorUINode();
        
        void onUnloadScene() override;
        void render() override;
    private:
        std::unique_ptr<ComponentUIManager> component_ui_manager;
    };
}
