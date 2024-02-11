#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <Match/vulkan/renderer.hpp>

namespace MatchEngine {
    // Vulkan的Subpass
    class Subpass {
        NoCopyMoveConstruction(Subpass)
    public:
        Subpass(const std::string &name) : name(name) {}
        virtual ~Subpass() = default;

        virtual void createAttachent(Match::RenderPassBuilder &builder) {}
        virtual void buildPassDescriptor(Match::SubpassBuilder &builder) = 0;
        virtual void start(std::shared_ptr<Match::Renderer> renderer) {}
        virtual void executePreRenderPass(std::shared_ptr<Match::Renderer> renderer) {}
        virtual void executeRenderPass(std::shared_ptr<Match::Renderer> renderer) {}
        virtual void executePostRenderPass(std::shared_ptr<Match::Renderer> renderer) {}

        std::string getName() const { return name; }
    protected:
        std::string name;
    };
}
