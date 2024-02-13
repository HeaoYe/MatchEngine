#include <ui_nodes/inspector_ui_node.hpp>

namespace MatchEditor {
    void InspectorUINode::render() {
        ImGui::Begin("Inspector");

        if (global_ui_context->selected_game_object_uuid == MatchEngine::Game::GameObjectUUID(-1)) {
            ImGui::End();
            return;
        }

        auto *game_object = MatchEngine::global_runtime_context->scene_manager->active_scene->game_object_map[global_ui_context->selected_game_object_uuid];
        for (auto *component : game_object->getComponets()) {
            if (ImGui::TreeNodeEx(component->getClassName().c_str())) {
                bool changed = false;
                for (auto &member_name : component->getMemberNames()) {
                    auto &member_desc = component->getMember(member_name);
                    switch (member_desc.getType()) {
                    case MatchEngine::MemberType::eInt:
                        changed |= ImGui::SliderInt(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<int>(component), -10, 10);
                        break;
                    case MatchEngine::MemberType::eFloat:
                        changed |= ImGui::SliderFloat(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<float>(component), -10, 10);
                        break;
                    case MatchEngine::MemberType::eVec2:
                        changed |= ImGui::SliderFloat2(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<glm::vec2>(component).x, -10, 10);
                        break;
                    case MatchEngine::MemberType::eVec3:
                        changed |= ImGui::SliderFloat3(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<glm::vec3>(component).x, -10, 10);
                        break;
                    case MatchEngine::MemberType::eVec4:
                        changed |= ImGui::SliderFloat4(member_name.c_str(), &member_desc.getValueMutReferenceByPtr<glm::vec4>(component).x, -10, 10);
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
                ImGui::TreePop();
                if (changed) {
                    component->onMemberUpdate();
                }
            }
        }

        ImGui::End();
    }
}
