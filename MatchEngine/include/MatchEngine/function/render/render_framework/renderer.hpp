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

        void pickGameObject(uint32_t x, uint32_t y, std::function<void(Game::GameObjectUUID uuid)> picked_callback, std::function<void()> miss_callback = []() {});
        void reportSelectedGameObject(Game::GameObjectUUID uuid = Game::GameObjectUUID(-1));
    MATCHEDITOR_VISIBLE:
        std::unique_ptr<Resource> resource;
        std::shared_ptr<Match::Renderer> renderer;
        std::vector<std::unique_ptr<Subpass>> subpasses;
        std::unique_ptr<GameObjectPicker> picker;
    };
}
