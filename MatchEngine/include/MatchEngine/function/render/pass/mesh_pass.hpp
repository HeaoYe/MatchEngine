#pragma once

#include <MatchEngine/function/render/pass/subpass.hpp>
#include <Match/vulkan/resource/buffer.hpp>

namespace MatchEngine {
    // 渲染Mesh的Subpass
    class MeshPass final : public Subpass {
        NoCopyMoveConstruction(MeshPass)
    public:
        MeshPass();
        ~MeshPass() override;

        void createAttachent(Match::RenderPassBuilder &builder) override;
        void buildPassDescriptor(Match::SubpassBuilder &builder) override;
        void start(std::shared_ptr<Match::Renderer> renderer) override;
        void executePreRenderPass(std::shared_ptr<Match::Renderer> renderer) override;
        void executeRenderPass(std::shared_ptr<Match::Renderer> renderer) override;
    private:
        std::shared_ptr<Match::InFlightBuffer> counts_buffer;
        std::shared_ptr<Match::InFlightBuffer> primitive_counts_buffer;
        std::shared_ptr<Match::InFlightBuffer> visible_mesh_instance_indices_buffer;
        std::shared_ptr<Match::InFlightBuffer> indirect_commands_buffer;
        std::shared_ptr<Match::InFlightBuffer> available_indirect_commands_buffer;
        std::shared_ptr<Match::InFlightBuffer> instance_locations_buffer;
        std::shared_ptr<Match::InFlightBuffer> instance_rotations_buffer;
        std::shared_ptr<Match::InFlightBuffer> instance_scales_buffer;

        std::vector<vk::CommandBuffer> command_buffers;
    MATCHEDITOR_VISIBLE:
        std::vector<uint32_t *> counts_ptrs;
    private:
        std::vector<vk::Fence> compute_fences;

        std::shared_ptr<Match::DescriptorSet> compute_shader_program_descriptor_set;
        std::shared_ptr<Match::PushConstants> compute_shader_constants;
        std::shared_ptr<Match::ComputeShaderProgram> collect_mesh_instance_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> generate_indirect_command_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> compact_instance_data_shader_program;
        std::shared_ptr<Match::ComputeShaderProgram> collect_available_indirect_command_shader_program;

        std::shared_ptr<Match::DescriptorSet> mesh_shader_program_descriptor_set;
        std::shared_ptr<Match::GraphicsShaderProgram> mesh_shader_program;
    };
}
