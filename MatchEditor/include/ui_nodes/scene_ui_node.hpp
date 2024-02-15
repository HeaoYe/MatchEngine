#pragma once

#include <ui.hpp>

namespace MatchEditor {
    // 场景预览器
    class SceneUINode : public UINode {
    public:
        SceneUINode();
        ~SceneUINode();
        void render() override;
    private:
        std::shared_ptr<Match::Sampler> sampler;
        std::vector<VkDescriptorSet> in_flight_textures;
        uint32_t callback_id;
        bool scene_focus;
        bool scene_hover;
    };
}
