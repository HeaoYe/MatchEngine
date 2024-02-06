#include <MatchEngine/game_framework/component/location/location_component.hpp>
#include <MatchEngine/game_framework/game_object/game_object.hpp>

namespace MatchEngine::Game {
    void LocationComponent::onCreate() {
        location = { 0, 0, 0 };
    }

    void LocationComponent::onDestroy() {}
    
    void LocationComponent::onClone(Component *rhs) const {
        static_cast<LocationComponent *>(rhs)->location = location;
    }

    void LocationComponent::onTick(float dt) {}
}
