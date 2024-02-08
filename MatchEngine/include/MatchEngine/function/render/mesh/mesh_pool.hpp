#pragma once

#include <MatchEngine/function/render/mesh/vertex_buffer_binder.hpp>
#include <MatchEngine/function/render/mesh/mesh_raw_data.hpp>
#include <MatchEngine/function/render/mesh/mesh_instance.hpp>

namespace MatchEngine {
    // Mesh池 管理一个场景中的所有Mesh, 在场景开始时会把所有Mesh的实例合批
    class MeshPool : public VertexBufferBinder {
        NoCopyMoveConstruction(MeshPool)
        friend class SceneRenderer;
    private:
        struct MeshInstanceCollect {
            uint32_t instance_count;

            std::vector<glm::vec3> locations;
            std::vector<glm::vec3> rotations;
            std::vector<glm::vec3> scales;
        };
    public:
        MeshPool(uint64_t vertex_memory_size, uint64_t index_memory_size);
        ~MeshPool();
        MeshID uploadMeshRawData(const MeshRawData *data, uint32_t max_instance_count = 2);
        void addMeshInstance(const MeshInstance &instance);
    private:
        void build();
        void bind(std::shared_ptr<Match::Renderer> renderer) override;
    private:
        void onBuildInputBindings(std::vector<Match::InputBindingInfo> &binding_infos) override;
    private:
        uint32_t vertex_offset;
        uint32_t index_offset;
        uint32_t max_vertex_offset;
        uint32_t max_index_offset;

        std::shared_ptr<Match::VertexBuffer> position_buffer;
        std::shared_ptr<Match::VertexBuffer> normal_buffer;
        std::shared_ptr<Match::VertexBuffer> tex_coord_buffer;
        std::shared_ptr<Match::VertexBuffer> color_buffer;
        std::shared_ptr<Match::IndexBuffer> index_buffer;

        std::shared_ptr<Match::VertexBuffer> instance_location_buffer;
        std::shared_ptr<Match::VertexBuffer> instance_rotation_buffer;
        std::shared_ptr<Match::VertexBuffer> instance_scale_buffer;

        std::vector<MeshID> exist_meshs;
        std::map<MeshID, Mesh> meshs;
        std::map<MeshID, MeshInstanceCollect> mesh_instance_collects;

        std::unique_ptr<Match::TwoStageBuffer> indirect_command_buffer;
        uint32_t indirect_command_count;
    };
}
