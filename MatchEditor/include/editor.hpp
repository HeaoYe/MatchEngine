#pragma once

#include <MatchEngine/engine/engine.hpp>
#include <ui.hpp>
#include <memory>

namespace MatchEditor {
    // MatchEditor 游戏引擎编辑器
    class MatchEditor {
    public:
        MatchEditor(MatchEngine::MatchEngine *engine);
        void initialize();
        void destroy();
        ~MatchEditor();
    private:
        MatchEngine::MatchEngine *engine;
        std::unique_ptr<UI> ui;
    };
}
