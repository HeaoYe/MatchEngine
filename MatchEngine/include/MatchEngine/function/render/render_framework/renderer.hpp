#pragma once

#include <MatchEngine/function/render/render_framework/mesh_pass.hpp>
#include <Match/vulkan/renderer.hpp>

namespace MatchEngine::Renderer {
    class Renderer {
        NoCopyMoveConstruction(Renderer)
    public:
        Renderer();
        ~Renderer();

        void render();
    MATCHEDITOR_VISIBLE:
        std::unique_ptr<Resource> resource;
        std::shared_ptr<Match::Renderer> renderer;
        std::vector<std::unique_ptr<Subpass>> subpasses;
    };
}
