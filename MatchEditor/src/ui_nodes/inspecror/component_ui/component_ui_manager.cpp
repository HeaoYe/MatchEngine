#include <ui_nodes/inspector/component_ui/component_ui_manager.hpp>
#include <ui_nodes/inspector/component_ui/component/transform/transform.hpp>
#include <ui_nodes/inspector/component_ui/component/camera/perspective_camera_component.hpp>
#include <imgui.h>

namespace MatchEditor {
    ComponentUIManager::ComponentUIManager() {
        registerComponentUI<MatchEngine::Game::PerspectiveCameraComponent>();
    }
    
    ComponentUIManager::~ComponentUIManager() {
        clearCache();
        for (auto &[name, ui_wrapper] : ui_wrappers) {
            ui_wrapper.delete_callback();
        }
    }
    
    void ComponentUIManager::renderComponent(MatchEngine::Game::Component *component) {
        auto ui_iter = ui_wrappers.find(component->getClassName());
        if (ui_iter == ui_wrappers.end()) {
            defaultRenderComponent(component);
        } else {
            ui_iter->second.render_callback(component);
        }
    }
 
    void ComponentUIManager::clearCache() {
        for (auto &[uuid, view_wrappers] : component_view_cache) {
            for (auto &[name, view_wrapper] : view_wrappers) {
                view_wrapper.delete_callback();
            }
        }
        component_view_cache.clear();
    }
 
    void ComponentUIManager::defaultRenderComponent(MatchEngine::Game::Component *component) {
        ImGui::PushID(size_t(component));
        if (ImGui::TreeNodeEx(component->getClassName().c_str())) {
            bool changed = false;
            for (auto &member_name : component->getMemberNames()) {
                auto &member_desc = component->getMember(member_name);
                switch (member_desc.getType()) {
                case MatchEngine::MemberType::eInt:
                    changed |= ImGui::DragInt(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<int>(component));
                    break;
                case MatchEngine::MemberType::eFloat:
                    changed |= ImGui::DragFloat(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<float>(component), 0.1);
                    break;
                case MatchEngine::MemberType::eBool:
                    changed |= ImGui::Checkbox(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<bool>(component));
                    break;
                case MatchEngine::MemberType::eVec2:
                    changed |= ImGui::DragFloat2(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<glm::vec2>(component).x, 0.1);
                    break;
                case MatchEngine::MemberType::eVec3:
                    changed |= ImGui::DragFloat3(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<glm::vec3>(component).x, 0.1);
                    break;
                case MatchEngine::MemberType::eVec4:
                    changed |= ImGui::DragFloat4(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<glm::vec4>(component).x, 0.1);
                    break;
                case MatchEngine::MemberType::eColor3:
                    changed |= ImGui::ColorEdit3(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<MatchEngine::Game::color3>(component).r);
                    break;
                case MatchEngine::MemberType::eColor4:
                    changed |= ImGui::ColorEdit4(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<MatchEngine::Game::color4>(component).r);
                    break;
                case MatchEngine::MemberType::eString:
                    ImGui::Text("%s: %s", member_name.c_str(), member_desc.getValueMutReferenceByPtr<std::string>(component).c_str());
                    break;
                case MatchEngine::MemberType::eCustom:
                    ImGui::Text("%s: Custom Type", member_name.c_str());
                }
            }
            if (changed) {
                component->onMemberUpdate();
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}
