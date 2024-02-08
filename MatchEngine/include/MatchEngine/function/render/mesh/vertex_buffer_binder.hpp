#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <Match/vulkan/renderer.hpp>

namespace MatchEngine {
    // VertexBuffer绑定器
    class VertexBufferBinder {
        DefaultNoCopyMoveConstruction(VertexBufferBinder)
    public:
        void buildInputBindings(std::vector<Match::InputBindingInfo> &binding_infos);
        virtual void bind(std::shared_ptr<Match::Renderer> renderer) = 0;
    protected:
        virtual void onBuildInputBindings(std::vector<Match::InputBindingInfo> &binding_infos) = 0;
    protected:
        uint32_t first_binding { -1u };
    };
}
