#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>
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
    MATCHEDITOR_VISIBLE:
        std::unique_ptr<Resource> resource;
        std::shared_ptr<Match::Renderer> renderer;
        std::vector<std::unique_ptr<Subpass>> subpasses;
    };
}
