#pragma once

#include <Match/vulkan/resource/buffer.hpp>
#include <Match/vulkan/resource/image.hpp>

namespace MatchEngine::Renderer {
    // 全局共享渲染资源
    struct Resource {
        // Render State 渲染状态
        uint32_t visibility_count { 0 };
        uint32_t draw_call_count { 0 };
        // ------------

        // Renderer 渲染器同步参数
        std::vector<std::vector<vk::PipelineStageFlags>> in_flight_wait_stages;
        std::vector<std::vector<vk::Semaphore>> in_flight_wait_semaphore;
        // ------------

        // CullingPass 剔除管线资源
        vk::Sampler depth_sampler;
        std::vector<std::shared_ptr<Match::Image>> depth_textures;  // 多级深度缓存
        std::vector<std::vector<vk::ImageView>> depth_texture_mipmap_views;
        std::vector<vk::ImageView> depth_texture_views;
        uint32_t depth_mip_level_count;
        std::shared_ptr<Match::InFlightBuffer> counts_buffer;
        std::shared_ptr<Match::InFlightBuffer> visible_mesh_instance_indices_buffer;
        std::shared_ptr<Match::InFlightBuffer> primitive_counts_buffer;
        std::shared_ptr<Match::InFlightBuffer> indirect_commands_buffer;
        std::shared_ptr<Match::InFlightBuffer> available_indirect_commands_buffer;
        std::shared_ptr<Match::InFlightBuffer> instance_datas_buffer;

        std::vector<vk::CommandBuffer> culling_command_buffers;
        std::vector<vk::Semaphore> culling_finish_semaphores;
        // ------------

        // LightPass
        uint tile_pixel_size;
        uint32_t tile_x, tile_y;
        std::shared_ptr<Match::InFlightBuffer> tile_point_light_counts_buffer;
        std::shared_ptr<Match::InFlightBuffer> tile_point_light_indices_buffer;
        std::shared_ptr<Match::InFlightBuffer> tile_view_z_buffer;
        // ------------

        // OutliningPass
        uint32_t selected_mesh_instance_index;
        std::shared_ptr<Match::InFlightBuffer> outlining_buffer;
        // ------------
    };
}
