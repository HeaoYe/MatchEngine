#pragma once

#include <MatchEngine/function/render/pass/subpass.hpp>
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
        virtual void preSceneRendererStart() {}
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

    class ImGuiPass : public MatchEngine::Subpass {
        NoCopyMoveConstruction(ImGuiPass)
    public:
        ImGuiPass(UI *ui) : MatchEngine::Subpass("ImGui Layer"), ui(ui) {}
        void buildPassDescriptor(Match::SubpassBuilder &builder) override {}
        void executeRenderPass(std::shared_ptr<Match::Renderer> renderer) override;
    private:
        UI *ui;
    };
}
