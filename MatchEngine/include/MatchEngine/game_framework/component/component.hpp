#pragma once

#include <MatchEngine/game_framework/rtti.hpp>
#include <MatchEngine/core/reflect/reflect_macro.hpp>
#include <MatchEngine/game_framework/uuid_type.hpp>
#include <MatchEngine/game_framework/types.hpp>

namespace MatchEngine::Game {
    // 组件基类, 所有组件继承自这个类
    REFLECT_CLASS(NAME = "Component")
    class Component : public RTTI {
        DefaultNoCopyMoveConstruction(Component)
        DECLARE_RTTI(Component)
        friend class GameObject;
    public:
        virtual ~Component() = default;

        // 组件接口, 可以选择部分实现

        // 在进入游戏循环前调用一次onStart
        virtual void onStart() {};
        // 以固定速率进行tick
        virtual void onFixedTick() {};
        // 每帧tick
        virtual void onTick(float dt) {};
        // 每帧post tick
        virtual void onPostTick(float dt) {};
        // GameObject销毁时会调用它的所有Component的onDestroy
        virtual void onDestroy() {};
    public:
        uint32_t registerMemberUpdateCallback(std::function<void(Component *)> callback) { member_update_callbacks.insert(std::make_pair(current_member_update_callback_id, callback)); current_member_update_callback_id += 1; return current_member_update_callback_id - 1; }
        void removeMemberUpdataCallback(uint32_t callback_id) { member_update_callbacks.erase(callback_id); }
        void onMemberUpdate() { for (auto &[id, callback] : member_update_callbacks) { callback(this); } };
        ComponentTypeUUID getTypeUUID() const { return uuid; }
    protected:
        class GameObject *master;  // set by game_object
        ComponentTypeUUID uuid;    // set by game_object
        uint32_t current_member_update_callback_id { 0 };
        std::map<uint32_t, std::function<void(Component *)>> member_update_callbacks;
    };

    #define DECLARE_COMPONENT_CUSTOM_CONSTRUCTION(class_name) \
        NoCopyMoveConstruction(class_name) \
        DECLARE_RTTI(class_name) \
    private: \

    #define DECLARE_COMPONENT(class_name) \
        DefaultNoCopyMoveConstruction(class_name) \
        DECLARE_RTTI(class_name) \
    private: \

}
