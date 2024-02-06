#pragma once

#include <MatchEngine/game_framework/component/component.hpp>
#include <MatchEngine/function/user_interface/user_interface.hpp>

#define DECLARE_COMPONENT(class_name) \
    NoCopyMoveConstruction(class_name) \
    DECLARE_RTTI(class_name) \
public: \
    class_name() { uuid = ::MatchEngine::UserInterface::component_type_uuid_system->getUUID<class_name>(); setupRTTI(); } \
    ::MatchEngine::Game::Component *onNewComponent() const override { return new class_name(); } \
private: \

#define DECLARE_COMPONENT_OVERRIDES() \
    void onCreate() override; \
    void onDestroy() override; \
    void onClone(::MatchEngine::Game::Component *rhs) const override; \
    void onTick(float dt) override; \
