#include <MatchEngine/function/render/render_framework/mesh_pass.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    void MeshPass::buildPassDescriptor(Match::SubpassBuilder &builder) {
        builder.attach_input_attachment("visibility buffer")
            .attach_output_attachment(global_runtime_context->render_system->output_attachment_name)
            .wait_for(
                "visibility pass",
                {
                    .stage = vk::PipelineStageFlagBits::eFragmentShader,
                    .access = vk::AccessFlagBits::eColorAttachmentRead,
                },
                {
                    .stage = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                    .access = vk::AccessFlagBits::eColorAttachmentWrite,
                }
            );
    }

    void MeshPass::postCreateRenderResource(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto manager = global_runtime_context->render_system->getMatchAPIManager();
        auto factory = global_runtime_context->render_system->getMatchFactory();

        constants = factory->create_push_constants(
            Match::ShaderStage::eFragment,
            {
                { "tile_size", Match::ConstantType::eUint32x2 },
            });
        glm::uvec2 tile_size = { resource.tile_x, resource.tile_y };
        constants->push_constant("tile_size", &tile_size);

        mesh_descriptor_set = factory->create_descriptor_set(renderer);
        mesh_descriptor_set->add_descriptors({
            { Match::ShaderStage::eFragment, 0, Match::DescriptorType::eInputAttachment },
            { Match::ShaderStage::eFragment, 1, Match::DescriptorType::eUniform },
            { Match::ShaderStage::eFragment, 2, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 3, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 4, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 5, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 6, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 7, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 8, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 9, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 10, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 11, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eFragment, 12, Match::DescriptorType::eStorageBuffer },
        }).allocate()
            .bind_input_attachment(0, "visibility buffer", factory->create_sampler({
                .mag_filter = Match::SamplerFilter::eNearest,
                .min_filter = Match::SamplerFilter::eNearest,
                .address_mode_u = Match::SamplerAddressMode::eClampToEdge,
                .address_mode_v = Match::SamplerAddressMode::eClampToEdge,
                .address_mode_w = Match::SamplerAddressMode::eClampToEdge,
            }))
            .bind_uniform(1, global_runtime_context->camera_system->getViewportCamera())
            .bind_storage_buffer(2, global_runtime_context->assets_system->getMeshPool()->index_buffer)
            .bind_storage_buffer(3, global_runtime_context->assets_system->getMeshPool()->position_buffer)
            .bind_storage_buffer(4, global_runtime_context->assets_system->getMeshPool()->normal_buffer)
            .bind_storage_buffer(5, global_runtime_context->assets_system->getMeshPool()->tex_coord_buffer)
            .bind_storage_buffer(6, global_runtime_context->assets_system->getMeshPool()->color_buffer)
            .bind_storage_buffer(7, resource.instance_datas_buffer)
            .bind_storage_buffer(8, resource.counts_buffer)
            .bind_storage_buffer(9, resource.available_indirect_commands_buffer)
            .bind_storage_buffer(10, resource.tile_point_light_counts_buffer)
            .bind_storage_buffer(11, resource.tile_point_light_indices_buffer)
            .bind_storage_buffer(12, global_runtime_context->render_system->getSwapData()->getPointLightPool()->point_light_buffer);

        auto vert_shader = factory->compile_shader(getName() + "/shader.vert", Match::ShaderStage::eVertex);
        auto frag_shader = factory->compile_shader(getName() + "/shader.frag", Match::ShaderStage::eFragment);
        mesh_shader_program = factory->create_shader_program(renderer, name);
        mesh_shader_program->attach_vertex_shader(vert_shader)
            .attach_fragment_shader(frag_shader)
            .attach_push_constants(constants)
            .attach_descriptor_set(mesh_descriptor_set)
            .compile({
                .cull_mode = Match::CullMode::eNone,
                .depth_test_enable = VK_FALSE,
            });
    }

    void MeshPass::executeRenderPass(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto command_buffer = renderer->get_command_buffer();
        renderer->bind_shader_program(mesh_shader_program);
        command_buffer.draw(3, 1, 0, 0);
    }
}
