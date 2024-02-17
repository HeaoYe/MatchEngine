#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/game_framework/uuid_type.hpp>
#include <MatchEngine/function/render/render_framework/resource.hpp>
#include <Match/vulkan/resource/shader_program.hpp>

namespace MatchEngine::Renderer {
    class GameObjectPicker {
        NoCopyMoveConstruction(GameObjectPicker)
    private:
        struct PickTask {
            uint32_t x, y;
            std::function<void(Game::GameObjectUUID uuid)> picked_callback;
            std::function<void()> miss_callback;
        };
    public:
        GameObjectPicker(std::shared_ptr<Match::Renderer> renderer, Resource &resource);
        void addPickTask(const PickTask &task);
        void processPickTasks(std::shared_ptr<Match::Renderer> renderer, uint32_t in_flight_idnex);
        bool needProcess() const { return !pick_tasks.empty(); }
    private:
        std::shared_ptr<Match::Buffer> pick_result_buffer;
        std::shared_ptr<Match::DescriptorSet> descriptor_set;
        std::shared_ptr<Match::PushConstants> constants;
        std::shared_ptr<Match::ComputeShaderProgram> pick_game_object_shader_program;
        std::vector<PickTask> pick_tasks;
    };
}
