#include <MatchEngine/function/render/mesh/mesh_instance_pool.hpp>

namespace MatchEngine {
    MeshInstancePool::MeshInstancePool(uint32_t max_mesh_instance_count) : current_instance_count(0) {
        mesh_instance_buffer = std::make_shared<Match::Buffer>(sizeof(MeshInstance) * max_mesh_instance_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        mesh_instance_buffer_ptr = static_cast<MeshInstance *>(mesh_instance_buffer->map());
    }
    
    MeshInstancePool::~MeshInstancePool() {
        mesh_instance_buffer.reset();
    }

    void MeshInstancePool::createMeshInstance(const MeshInstance &mesh_instance) {
        *mesh_instance_buffer_ptr = mesh_instance;
        mesh_instance_buffer_ptr ++;
        current_instance_count ++;
    }
}
