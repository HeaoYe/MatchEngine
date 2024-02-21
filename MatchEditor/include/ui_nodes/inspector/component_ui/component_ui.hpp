#pragma once

#include <ui_nodes/inspector/component_ui/component_view.hpp>
#include <MatchEngine/../../src/MatchEngine/internal.hpp>
#include <ui_nodes/ui_context.hpp>
#include <imgui.h>

namespace MatchEditor {
    template <class ComponentType>
    class ComponentUI {
    public:
        void render(ComponentView<ComponentType> &view);
    };
}
