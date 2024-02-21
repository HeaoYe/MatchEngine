#include <MatchEngine/function/render/render_framework/game_object_picker.hpp>
#include "internal.hpp"

namespace MatchEngine::Renderer {
    GameObjectPicker::GameObjectPicker(std::shared_ptr<Match::Renderer> renderer, Resource &resource) {
        auto manager = global_runtime_context->render_system->getMatchAPIManager();
        auto factory = global_runtime_context->render_system->getMatchFactory();

        pick_result_buffer = std::make_shared<Match::Buffer>(sizeof(uint32_t), vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_TO_CPU, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);

        descriptor_set = factory->create_descriptor_set();
        descriptor_set->add_descriptors({
            { Match::ShaderStage::eCompute, 0, Match::DescriptorType::eStorageImage },
            { Match::ShaderStage::eCompute, 1, Match::DescriptorType::eStorageBuffer },
            { Match::ShaderStage::eCompute, 2, Match::DescriptorType::eStorageBuffer },
        }).allocate()
            .bind_storage_buffer(1, resource.instance_datas_buffer)
            .bind_storage_buffer(2, pick_result_buffer);

        auto id = renderer->register_resource_recreate_callback([ptr = renderer.get(), this]() {
            for (size_t in_flight_index = 0; in_flight_index < Match::setting.max_in_flight_frame; in_flight_index ++) {
                vk::DescriptorImageInfo image_info {};
                image_info.setImageLayout(vk::ImageLayout::eGeneral)
                    .setImageView(ptr->framebuffer_set->attachments.at(ptr->render_pass_builder->get_attachment_index("visibility buffer", true)).image_view);
                vk::WriteDescriptorSet descriptor_write {};
                descriptor_write.setDstSet(descriptor_set->descriptor_sets[in_flight_index])
                    .setDstBinding(0)
                    .setDstArrayElement(0)
                    .setDescriptorType(vk::DescriptorType::eStorageImage)
                    .setImageInfo(image_info);
                global_runtime_context->render_system->getMatchAPIManager()->device->device.updateDescriptorSets(descriptor_write, {});
            }
        });
        renderer->callbacks.at(id)();

        constants = factory->create_push_constants(
            Match::ShaderStage::eCompute,
            {
                { "pos", Match::ConstantType::eUint32x2 },
            });

        pick_game_object_shader_program = factory->create_compute_shader_program();
        pick_game_object_shader_program->attach_compute_shader(factory->compile_shader("pick_game_object.comp", Match::ShaderStage::eCompute))
            .attach_descriptor_set(descriptor_set)
            .attach_push_constants(constants)
            .compile();
    }

    void GameObjectPicker::addPickTask(const PickTask &task) {
        pick_tasks.push_back(task);
    }

    void GameObjectPicker::processPickTasks(std::shared_ptr<Match::Renderer> renderer, uint32_t in_flight_idnex) {
        for (auto &task : pick_tasks) {
            auto manager = global_runtime_context->render_system->getMatchAPIManager();
            glm::uvec2 pos { task.x, task.y };
            constants->push_constant("pos", &pos);
            auto command_buffer = manager->command_pool->allocate_single_use();
            vk::ImageMemoryBarrier image_barrier {};
            image_barrier.setImage(renderer->framebuffer_set->attachments.at(renderer->render_pass_builder->get_attachment_index("visibility buffer", true)).image->image)
                .setSubresourceRange({
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    0,
                    1,
                })
                .setOldLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setNewLayout(vk::ImageLayout::eGeneral)
                .setSrcAccessMask(vk::AccessFlagBits::eShaderRead)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
            command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eFragmentShader, vk::PipelineStageFlagBits::eComputeShader, {}, {}, {}, image_barrier);
            command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, pick_game_object_shader_program->pipeline);
            command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, pick_game_object_shader_program->layout, 0, descriptor_set->descriptor_sets[in_flight_idnex], {});
            command_buffer.pushConstants(pick_game_object_shader_program->layout, vk::ShaderStageFlagBits::eCompute, 0, constants->constants_size, constants->constants.data());
            command_buffer.dispatch(1, 1, 1);
            image_barrier.setOldLayout(vk::ImageLayout::eGeneral)
                .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setSrcAccessMask(vk::AccessFlagBits::eShaderRead)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
            command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {}, image_barrier);
            manager->command_pool->free_single_use(command_buffer);
            auto mesh_instance_index = *static_cast<uint32_t *>(pick_result_buffer->map());
            if (mesh_instance_index == uint32_t(-1)) {
                task.miss_callback();
            } else {
                task.picked_callback(global_runtime_context->render_system->getSwapData()->getMeshInstancePool()->mesh_instance_index_to_game_object_uuid_map.at(mesh_instance_index));
            }
        }
        pick_tasks.clear();
    }
}
