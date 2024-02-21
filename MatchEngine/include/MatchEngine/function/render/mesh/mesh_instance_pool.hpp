#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/function/render/mesh/mesh_instance.hpp>
#include <MatchEngine/game_framework/uuid_type.hpp>
#include <Match/vulkan/resource/buffer.hpp>

namespace MatchEngine {
    class MeshInstancePool {
        NoCopyMoveConstruction(MeshInstancePool)
    public:
        MeshInstancePool(uint32_t max_mesh_instance_count);

        void createMeshInstance(const MeshInstance &mesh_instance, Game::GameObjectUUID uuid);
        MeshInstance *getMeshInstancePtr(Game::GameObjectUUID uuid);
        void clear();
    INNER_VISIBLE:
        uint32_t current_instance_count;
        std::shared_ptr<Match::Buffer> mesh_instance_buffer;
        std::map<Game::GameObjectUUID, uint32_t> game_object_uuid_to_mesh_instance_index_map;
        std::map<uint32_t, Game::GameObjectUUID> mesh_instance_index_to_game_object_uuid_map;
        MeshInstance *mesh_instance_buffer_ptr;
    };
}
