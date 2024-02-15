#include <MatchEngine/function/render/mesh/mesh_instance_pool.hpp>

namespace MatchEngine {
    MeshInstancePool::MeshInstancePool(uint32_t max_mesh_instance_count) : current_instance_count(0) {
        mesh_instance_buffer = std::make_shared<Match::Buffer>(sizeof(MeshInstance) * max_mesh_instance_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        mesh_instance_buffer_ptr = static_cast<MeshInstance *>(mesh_instance_buffer->map());
    }
    
    MeshInstancePool::~MeshInstancePool() {
        mesh_instance_buffer.reset();
    }

    uint32_t MeshInstancePool::createMeshInstance(const MeshInstance &mesh_instance) {
        *mesh_instance_buffer_ptr = mesh_instance;
        mesh_instance_buffer_ptr ++;
        current_instance_count ++;
        return current_instance_count - 1;
    }

    MeshInstance *MeshInstancePool::getMeshInstancePtr(uint32_t mesh_instance_index) {
        return static_cast<MeshInstance *>(mesh_instance_buffer->map()) + mesh_instance_index;
    }
 
    void MeshInstancePool::clear() {
        mesh_instance_buffer_ptr = static_cast<MeshInstance *>(mesh_instance_buffer->map());
        current_instance_count = 0;
        memset(mesh_instance_buffer_ptr, 0, mesh_instance_buffer->size);
    }
}
