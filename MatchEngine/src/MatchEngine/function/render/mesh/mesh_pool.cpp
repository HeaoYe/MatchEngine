#include <MatchEngine/function/render/mesh/mesh_pool.hpp>
#include "internal.hpp"

namespace MatchEngine {
    MeshPool::MeshPool(uint64_t vertex_memory_size, uint64_t index_memory_size) : vertex_offset(0), index_offset(0) {
        auto factory = global_runtime_context->render_system->getMatchFactory();
        max_vertex_offset = vertex_memory_size / sizeof(glm::vec3);
        max_index_offset = index_memory_size / sizeof(uint32_t);

        position_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), max_vertex_offset);
        normal_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), max_vertex_offset);
        tex_coord_buffer = factory->create_vertex_buffer(sizeof(glm::vec2), max_vertex_offset);
        color_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), max_vertex_offset);
        index_buffer = factory->create_index_buffer(Match::IndexType::eUint32, max_index_offset);
    }

    MeshPool::~MeshPool() {
        meshs.clear();
        position_buffer.reset();
        normal_buffer.reset();
        tex_coord_buffer.reset();
        color_buffer.reset();
        index_buffer.reset();
    }

    MeshID MeshPool::uploadMeshRawData(const MeshRawData *data, uint32_t max_instance_count) {
        if (vertex_offset + data->positions.size() > max_vertex_offset) {
            MCH_ERROR("Mesh Vertex Buffer Out Of Memory")
            return MeshID(-1);
        }
        if (index_offset + data->indices.size() > max_index_offset) {
            MCH_ERROR("Mesh Index Buffer Out Of Memory")
            return MeshID(-1);
        }
        position_buffer->upload_data_from_vector(data->positions, vertex_offset);
        normal_buffer->upload_data_from_vector(data->normals, vertex_offset);
        tex_coord_buffer->upload_data_from_vector(data->tex_coords, vertex_offset);
        color_buffer->upload_data_from_vector(data->colors, vertex_offset);
        index_buffer->upload_data_from_vector(data->indices, index_offset);
        uint32_t first_instance = 0;
        for (auto [mesh_id, mesh] : meshs) {
            first_instance += mesh.max_instance_count;
        }
        exist_meshs.push_back(meshs.size());
        meshs.insert(std::make_pair(
            exist_meshs.back(),
            Mesh {
                .first_instance = first_instance,
                .max_instance_count = max_instance_count,
                .vertex_offset = vertex_offset,
                .first_index = index_offset,
                .index_count = static_cast<uint32_t>(data->indices.size())
            }
        ));
        mesh_instance_collects.insert(std::make_pair(
            exist_meshs.back(),
            MeshInstanceCollect {
                .instance_count = 0,
                .locations = std::vector<glm::vec3>(max_instance_count),
                .rotations = std::vector<glm::vec3>(max_instance_count),
                .scales = std::vector<glm::vec3>(max_instance_count)
            }
        ));
        vertex_offset += data->positions.size();
        index_offset += data->indices.size();
        return exist_meshs.back();
    }

    void MeshPool::addMeshInstance(const MeshInstance &instance) {
        if (auto iter = mesh_instance_collects.find(instance.mesh_id); iter != mesh_instance_collects.end()) {
            if (iter->second.instance_count >= meshs.at(instance.mesh_id).max_instance_count) {
                MCH_CORE_ERROR("Mesh {} instance count >= max instance count {},", instance.mesh_id, meshs.at(instance.mesh_id).max_instance_count);
                return;
            }
            iter->second.locations[iter->second.instance_count] = instance.location;
            iter->second.rotations[iter->second.instance_count] = instance.rotation;
            iter->second.scales[iter->second.instance_count] = instance.scale;
            iter->second.instance_count ++;
            return;
        }
        MCH_CORE_ERROR("Mesh {} doesn't exist.", instance.mesh_id)
    }

    void MeshPool::build() {
        uint32_t instance_count = 0;
        for (auto [mesh_id, mesh_instance_collect] : mesh_instance_collects) {
            instance_count += mesh_instance_collect.instance_count;
        }
        auto factory = global_runtime_context->render_system->getMatchFactory();
        instance_location_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), instance_count);
        instance_rotation_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), instance_count);
        instance_scale_buffer = factory->create_vertex_buffer(sizeof(glm::vec3), instance_count);
        for (auto mesh_id : exist_meshs) {
            auto &mesh_instance_collect = mesh_instance_collects.at(mesh_id);
            auto first_instance = meshs.at(mesh_id).first_instance;
            instance_location_buffer->upload_data_from_vector(mesh_instance_collect.locations, first_instance);
            instance_rotation_buffer->upload_data_from_vector(mesh_instance_collect.rotations, first_instance);
            instance_scale_buffer->upload_data_from_vector(mesh_instance_collect.scales, first_instance);
            mesh_instance_collect.locations.clear();
            mesh_instance_collect.rotations.clear();
            mesh_instance_collect.scales.clear();
        }
        std::vector<vk::DrawIndexedIndirectCommand> indirect_commands;
        for (auto mesh_id : exist_meshs) {
            auto &mesh = meshs.at(mesh_id);
            indirect_commands.emplace_back()
                .setFirstIndex(mesh.first_index)
                .setIndexCount(mesh.index_count)
                .setFirstInstance(mesh.first_instance)
                .setVertexOffset(mesh.vertex_offset)
                .setInstanceCount(mesh_instance_collects.at(mesh_id).instance_count);
        }
        indirect_command_count = indirect_commands.size();
        indirect_command_buffer = std::make_unique<Match::TwoStageBuffer>(sizeof(vk::DrawIndexedIndirectCommand) * indirect_command_count, vk::BufferUsageFlagBits::eIndirectBuffer);
        indirect_command_buffer->upload_data_from_vector(indirect_commands);
    }

    void MeshPool::bind(std::shared_ptr<Match::Renderer> renderer) {
        renderer->bind_vertex_buffers({
            position_buffer,
            normal_buffer,
            tex_coord_buffer,
            color_buffer,
            instance_location_buffer,
            instance_rotation_buffer,
            instance_scale_buffer,
        });
        renderer->bind_index_buffer(index_buffer);
    }

    void MeshPool::onBuildInputBindings(std::vector<Match::InputBindingInfo> &binding_infos) {
        binding_infos.push_back({
            .binding = first_binding,
            .rate = Match::InputRate::ePerVertex,
            .attributes = { Match::VertexType::eFloat3 }
        });
        binding_infos.push_back({
            .binding = first_binding + 1,
            .rate = Match::InputRate::ePerVertex,
            .attributes = { Match::VertexType::eFloat3 }
        });
        binding_infos.push_back({
            .binding = first_binding + 2,
            .rate = Match::InputRate::ePerVertex,
            .attributes = { Match::VertexType::eFloat2 }
        });
        binding_infos.push_back({
            .binding = first_binding + 3,
            .rate = Match::InputRate::ePerVertex,
            .attributes = { Match::VertexType::eFloat3 }
        });
        binding_infos.push_back({
            .binding = first_binding + 4,
            .rate = Match::InputRate::ePerInstance,
            .attributes = { Match::VertexType::eFloat3 }
        });
        binding_infos.push_back({
            .binding = first_binding + 5,
            .rate = Match::InputRate::ePerInstance,
            .attributes = { Match::VertexType::eFloat3 }
        });
        binding_infos.push_back({
            .binding = first_binding + 6,
            .rate = Match::InputRate::ePerInstance,
            .attributes = { Match::VertexType::eFloat3 }
        });
    }
}
