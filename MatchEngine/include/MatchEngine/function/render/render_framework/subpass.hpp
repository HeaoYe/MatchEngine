#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/function/render/render_framework/resource.hpp>
#include <Match/vulkan/renderer.hpp>

namespace MatchEngine::Renderer {
    // Vulkan的Subpass
    class Subpass {
        NoCopyMoveConstruction(Subpass)
    public:
        Subpass(const std::string &name) : name(name) {}
        virtual ~Subpass() = default;

        virtual void createRenderResource(Match::RenderPassBuilder &builder, Resource &resource) {}
        virtual void postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {}
        virtual void buildPassDescriptor(Match::SubpassBuilder &builder) {}
        virtual void executePreRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {}
        virtual void executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {}
        virtual void executePostRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {}

        std::string getName() const { return name; }
    protected:
        std::string name;
    };
}
