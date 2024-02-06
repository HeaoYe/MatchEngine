#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/core/reflect/reflect_macro.hpp>
#include <MatchEngine/game_framework/rtti.hpp>
#include <MatchEngine/game_framework/component/component_type_uuid.hpp>

namespace MatchEngine::Game {
    // 组件基类, 所有组件继承自这个类
    REFLECT_CLASS(NAME = "Component")
    class Component : public RTTI {
        DefaultNoCopyMoveConstruction(Component)
        DECLARE_RTTI(Component)
        friend class GameObjectResource;
        friend class GameObject;
    protected:
        virtual void onCreate() = 0;
        virtual void onDestroy() = 0;
        virtual void onClone(Component *rhs) const = 0;
        virtual Component *onNewComponent() const = 0;

        virtual void onTick(float dt) = 0;
    public:
        virtual ~Component() = default;
        Component *clone();
        void create();
        void destroy();
    public:
        ComponentTypeUUID getUUID() const { return uuid; }
    protected:
        class GameObject *master;
        ComponentTypeUUID uuid;
    };
}
