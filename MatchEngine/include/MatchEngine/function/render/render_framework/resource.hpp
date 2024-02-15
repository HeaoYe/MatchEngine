#pragma once

#include <Match/vulkan/resource/buffer.hpp>

namespace MatchEngine::Renderer {
    struct Resource {
        std::vector<uint32_t *> counts_ptrs;
        std::shared_ptr<Match::InFlightBuffer> counts_buffer;
        std::shared_ptr<Match::InFlightBuffer> primitive_counts_buffer;
        std::shared_ptr<Match::InFlightBuffer> visible_mesh_instance_indices_buffer;
        std::shared_ptr<Match::InFlightBuffer> indirect_commands_buffer;
        std::shared_ptr<Match::InFlightBuffer> available_indirect_commands_buffer;
        std::shared_ptr<Match::InFlightBuffer> instance_locations_buffer;
        std::shared_ptr<Match::InFlightBuffer> instance_rotations_buffer;
        std::shared_ptr<Match::InFlightBuffer> instance_scales_buffer;
    };
}
