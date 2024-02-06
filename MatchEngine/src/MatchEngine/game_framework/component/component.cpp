#include <MatchEngine/game_framework/component/component.hpp>

namespace MatchEngine::Game {
    Component *Component::clone() {
        Component *component = onNewComponent();
        onClone(component);
        return component;
    }
    
    void Component::create() {
        onCreate();
    }

    void Component::destroy() {
        onDestroy();
    }
};
