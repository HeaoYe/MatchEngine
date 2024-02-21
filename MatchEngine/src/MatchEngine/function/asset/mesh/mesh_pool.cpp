#include <MatchEngine/function/asset/mesh/mesh_pool.hpp>
#include "internal.hpp"

namespace MatchEngine {
    MeshPool::MeshPool(uint64_t vertex_memory_size, uint64_t index_memory_size, uint32_t max_mesh_descriptor_count, uint32_t max_primitive_count) {
        auto factory = global_runtime_context->render_system->getMatchFactory();
        current_vertex_count = 0;
        current_index_count = 0;
        max_vertex_count = vertex_memory_size / sizeof(glm::vec3);
        max_index_count = index_memory_size / sizeof(uint32_t);

        position_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), max_vertex_count, vk::BufferUsageFlagBits::eStorageBuffer);
        normal_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), max_vertex_count, vk::BufferUsageFlagBits::eStorageBuffer);
        tex_coord_buffer = factory->create_vertex_buffer(sizeof(glm::vec2), max_vertex_count, vk::BufferUsageFlagBits::eStorageBuffer);
        color_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), max_vertex_count, vk::BufferUsageFlagBits::eStorageBuffer);
        index_buffer = factory->create_index_buffer(Match::IndexType::eUint32, max_index_count, vk::BufferUsageFlagBits::eStorageBuffer);

        current_primitive_discriptor_count = 0;
        primitive_descriptor_buffer = std::make_shared<Match::Buffer>(sizeof(PrimitiveDescriptor) * max_primitive_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        primitive_descriptor_buffer_ptr = static_cast<PrimitiveDescriptor *>(primitive_descriptor_buffer->map());

        current_mesh_discriptor_count = 0;
        mesh_descriptor_buffer = std::make_shared<Match::Buffer>(sizeof(MeshDescriptor) * max_mesh_descriptor_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        mesh_descriptor_buffer_ptr = static_cast<MeshDescriptor *>(mesh_descriptor_buffer->map());
    }

    MeshPool::~MeshPool() {
        position_buffer.reset();
        normal_buffer.reset();
        tex_coord_buffer.reset();
        color_buffer.reset();
        index_buffer.reset();
    }

    MeshID MeshPool::uploadMeshData(const MeshData &data) {
        size_t vertex_count = 0, index_count = 0;
        for (auto &primitive : data.lods) {
            vertex_count += primitive.positions.size();
            index_count += primitive.indices.size();
        }
        if (current_vertex_count + vertex_count > max_vertex_count) {
            MCH_ERROR("Mesh Vertex Buffer Out Of Memory")
            return MeshID(-1);
        }
        if (current_index_count + index_count > max_index_count) {
            MCH_ERROR("Mesh Index Buffer Out Of Memory")
            return MeshID(-1);
        }

        mesh_descriptor_buffer_ptr->aabb_min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
        mesh_descriptor_buffer_ptr->aabb_max = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };
        mesh_descriptor_buffer_ptr->radius = 0;
        size_t lod_index = 0;
        for (auto &primitive : data.lods) {
            position_buffer->upload_data_from_vector(primitive.positions, current_vertex_count);
            normal_buffer->upload_data_from_vector(primitive.normals, current_vertex_count);
            tex_coord_buffer->upload_data_from_vector(primitive.tex_coords, current_vertex_count);
            color_buffer->upload_data_from_vector(primitive.colors, current_vertex_count);
            index_buffer->upload_data_from_vector(primitive.indices, current_index_count);
            auto primitive_id = current_primitive_discriptor_count;
            current_primitive_discriptor_count ++;

            primitive_descriptor_buffer_ptr->vertex_offset = current_vertex_count;
            primitive_descriptor_buffer_ptr->first_index = current_index_count;
            primitive_descriptor_buffer_ptr->index_count = primitive.indices.size();
            primitive_descriptor_buffer_ptr ++;
            current_vertex_count += primitive.positions.size();
            current_index_count += primitive.indices.size();
            mesh_descriptor_buffer_ptr->lods[lod_index] = primitive_id;
            lod_index ++;

            for (auto &position : primitive.positions) {
                mesh_descriptor_buffer_ptr->aabb_min = glm::min(mesh_descriptor_buffer_ptr->aabb_min, position);
                mesh_descriptor_buffer_ptr->aabb_max = glm::max(mesh_descriptor_buffer_ptr->aabb_max, position);
                mesh_descriptor_buffer_ptr->radius = std::max(mesh_descriptor_buffer_ptr->radius, glm::dot(position, position));
            }
        }
        mesh_descriptor_buffer_ptr->lod_count = data.lods.size();
        mesh_descriptor_buffer_ptr->radius = glm::sqrt(mesh_descriptor_buffer_ptr->radius);
        mesh_descriptor_buffer_ptr ++;
        current_mesh_discriptor_count ++;
        return current_mesh_discriptor_count - 1;
    }
}
