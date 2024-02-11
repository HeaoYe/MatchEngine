#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/function/render/mesh/mesh_instance.hpp>
#include <Match/vulkan/resource/buffer.hpp>

namespace MatchEngine {
    class MeshInstancePool {
        NoCopyMoveConstruction(MeshInstancePool)
        friend class MeshPass;
    public:
        MeshInstancePool(uint32_t max_mesh_instance_count);
        ~MeshInstancePool();
        void createMeshInstance(const MeshInstance &mesh_instance);
        size_t getMeshInstanceCount() const { return current_instance_count; }
    private:
        uint32_t current_instance_count;
        std::shared_ptr<Match::Buffer> mesh_instance_buffer;
        MeshInstance *mesh_instance_buffer_ptr;
    };
}
