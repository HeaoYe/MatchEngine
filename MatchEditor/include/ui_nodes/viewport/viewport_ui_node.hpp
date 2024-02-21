#pragma once

#include <ui.hpp>
#include <ui_nodes/viewport/viewport_camera.hpp>

namespace MatchEditor {
    // 场景预览器
    class ViewportUINode : public UINode {
    public:
        ViewportUINode();
        ~ViewportUINode();

        void onLoadScene() override;
        void onTick(float dt) override;
        void render() override;

        void bindOnSelectGameObject(std::function<void(MatchEngine::Game::GameObjectUUID uuid)> callback) { on_select_game_object_callback = callback; }
    private:
        std::shared_ptr<Match::Sampler> sampler;
        std::vector<VkDescriptorSet> in_flight_textures;
        uint32_t callback_id;
        bool viewport_focus;
        bool viewport_hover;
        ImVec2 cursor_screen_pos, frame_extent;
        std::function<void(MatchEngine::Game::GameObjectUUID uuid)> on_select_game_object_callback;
        std::unique_ptr<ViewportCamera> camera;
    };
}
