#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>
#include <MatchEngine/function/render/render_framework/game_object_picker.hpp>
#include <MatchEngine/game_framework/uuid_type.hpp>
#include <Match/vulkan/renderer.hpp>

namespace MatchEngine::Renderer {
    // 渲染器，引擎的渲染由这个Renderer管理，渲染有关资源都在MatchEngine::Renderer命名空间下
    class Renderer {
        NoCopyMoveConstruction(Renderer)
    public:
        Renderer();
        ~Renderer();

        void start();
        void render();

        // 添加一个拾取GameObject的任务, 包括x y坐标, 拾取到GameObject的回调函数, 没有拾取到的回调函数
        // 因为同步的问题, 会在一帧开始时处理拾取任务
        void pickGameObject(uint32_t x, uint32_t y, std::function<void(Game::GameObjectUUID uuid)> picked_callback, std::function<void()> miss_callback = []() {});

        // 报告当前拾取的GameObject, 用于高亮显示边框
        void reportSelectedGameObject(Game::GameObjectUUID uuid = Game::GameObjectUUID(-1));
    MATCHEDITOR_VISIBLE:
        std::unique_ptr<Resource> resource;
        std::shared_ptr<Match::Renderer> renderer;
        std::vector<std::unique_ptr<Subpass>> subpasses;
        std::unique_ptr<GameObjectPicker> picker;
    };
}
