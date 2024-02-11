#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/function/asset/mesh/primitive_data.hpp>
#include <MatchEngine/function/asset/mesh/mesh_id.hpp>
#include <MatchEngine/function/asset/mesh/mesh_descriptor.hpp>
#include <Match/vulkan/resource/buffer.hpp>

namespace MatchEngine {
    // Mesh池, 管理所有Mesh和Primitive(不同层次的Lod模型)
    class MeshPool {
        NoCopyMoveConstruction(MeshPool)
        friend class MeshPass;
    public:
        MeshPool(uint64_t vertex_memory_size, uint64_t index_memory_size, uint32_t max_mesh_descriptor_count, uint32_t max_primitive_descriptor_count);
        ~MeshPool();
        MeshID uploadMeshData(const MeshData &data);
        uint32_t getMeshCount() const { return current_mesh_discriptor_count; }
        uint32_t getPrimitiveCount() const { return current_primitive_discriptor_count; }
    private:
        uint32_t current_vertex_count;
        uint32_t current_index_count;
        uint32_t max_vertex_count;
        uint32_t max_index_count;

        std::shared_ptr<Match::VertexBuffer> position_buffer;
        std::shared_ptr<Match::VertexBuffer> normal_buffer;
        std::shared_ptr<Match::VertexBuffer> tex_coord_buffer;
        std::shared_ptr<Match::VertexBuffer> color_buffer;
        std::shared_ptr<Match::IndexBuffer> index_buffer;

        std::shared_ptr<Match::Buffer> primitive_descriptor_buffer;
        PrimitiveDescriptor *primitive_descriptor_buffer_ptr;
        uint32_t current_primitive_discriptor_count;

        std::shared_ptr<Match::Buffer> mesh_descriptor_buffer;
        MeshDescriptor *mesh_descriptor_buffer_ptr;
        uint32_t current_mesh_discriptor_count;
    };
}
