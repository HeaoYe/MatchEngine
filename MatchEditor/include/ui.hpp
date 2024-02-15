#pragma once

#include <MatchEngine/function/render/render_framework/subpass.hpp>
#include <MatchEngine/../../src/MatchEngine/internal.hpp>
#include <Match/vulkan/renderer.hpp>
#include <ui_nodes/ui_context.hpp>
#include <imgui.h>

namespace MatchEditor {
    // UI节点
    class UINode {
    public:
        UINode() = default;
        virtual ~UINode() = default;
        virtual void onLoadScene() {}
        virtual void onUnloadScene() {}
        virtual void render() = 0;
    };

    // 编辑器UI
    class UI {
    public:
        UI();
        void onLoadScene();
        void onUnloadScene();
        void render();
        ~UI();
    private:
        std::vector<std::unique_ptr<UINode>> ui_nodes;
    };

    class ImGuiPass : public MatchEngine::Renderer::Subpass {
        NoCopyMoveConstruction(ImGuiPass)
    public:
        ImGuiPass(UI *ui) : MatchEngine::Renderer::Subpass("ImGui Layer"), ui(ui) {}
        void executeRenderPass(std::shared_ptr<Match::Renderer> renderer, MatchEngine::Renderer::Resource &resource) override;
    private:
        UI *ui;
    };
}
