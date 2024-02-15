#include <ui_nodes/scene_ui_node.hpp>
#include <ui_nodes/hierarchy_ui_node.hpp>
#include <ui_nodes/inspector_ui_node.hpp>
#include <ui_nodes/setting_ui_node.hpp>
#include <Match/imgui/imgui.hpp>

namespace MatchEditor {
    void ImGuiPass::executeRenderPass(std::shared_ptr<Match::Renderer> renderer, MatchEngine::Renderer::Resource &resource) {
        renderer->begin_layer_render("ImGui Layer");

        ui->render();

        renderer->end_layer_render("ImGui Layer");
    }

    UI::UI() {
        Match::setting.font_size = 26;
        Match::setting.default_font_filename = "/usr/share/fonts/TTF/JetBrainsMonoNerdFontMono-Light.ttf";
        Match::setting.chinese_font_filename = "/usr/share/fonts/adobe-source-han-sans/SourceHanSansCN-Medium.otf";

        auto renderer = MatchEngine::global_runtime_context->render_system->getRenderer();
        renderer->subpasses.push_back(std::make_unique<ImGuiPass>(this));
        renderer->renderer->attach_render_layer<Match::ImGuiLayer>("ImGui Layer", std::vector { MatchEngine::Renderer::output_attachment_name });
        ImGui::LoadIniSettingsFromDisk("assets/ui/imgui.ini");
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        global_ui_context = new UIContext();
        ui_nodes.push_back(std::make_unique<SceneUINode>());
        ui_nodes.push_back(std::make_unique<HierarchyUINode>());
        ui_nodes.push_back(std::make_unique<InspectorUINode>());
        ui_nodes.push_back(std::make_unique<SettingUINode>());
    }

    void UI::onLoadScene() {
        for (auto &node : ui_nodes) {
            node->onLoadScene();
        }
    }

    void UI::onUnloadScene() {
        for (auto &node : ui_nodes) {
            node->onUnloadScene();
        }
    }

    void UI::render() {
        auto *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(3);
        ImGui::DockSpace(ImGui::GetID("DockSpace"), { 0.0f, 0.0f }, 0, nullptr);

        for (auto &node : ui_nodes) {
            node->render();
        }

        ImGui::End();
    }

    UI::~UI() {
        MatchEngine::global_runtime_context->render_system->waitRenderDevice();
        ui_nodes.clear();
        delete global_ui_context;
    }
}