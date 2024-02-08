#include <MatchEngine/function/render/render_target/render_target_pool.hpp>
#include "internal.hpp"

namespace MatchEngine {
    RenderTargetPool::RenderTargetPool(uint32_t width, uint32_t height) : width(width), height(height), output_render_target_id(-1), built(false) {
        auto factory = global_runtime_context->render_system->getMatchFactory();
        sampler = factory->create_sampler();
        uint8_t data[4] = { 0, 0, 0, 0 };
        empty_texture = factory->create_texture(data, 1, 1);
        render_target_descriptor_set = factory->create_descriptor_set();
    }

    RenderTargetPool::~RenderTargetPool() {
        render_target_descriptor_set.reset();
        empty_texture.reset();
        sampler.reset();
        render_targets.clear();
        exist_render_targets.clear();
    }

    RenderTargetID RenderTargetPool::createRenderTarget(RenderTargetFormat format) {
        if (built) {
            MCH_ERROR("Create RenderTarget after build.")
            return RenderTargetID(-1);
        }
        exist_render_targets.push_back(render_targets.size());
        render_targets.insert(std::make_pair(exist_render_targets.back(), std::make_unique<RenderTarget>(format, width, height)));
        if (output_render_target_id == RenderTargetID(-1)) {
            output_render_target_id = exist_render_targets.back();
        }
        return exist_render_targets.back();
    }
    
    void RenderTargetPool::setOutputRenderTarget(RenderTargetID id) {
        if (auto iter = render_targets.find(id); iter != render_targets.end()) {
            output_render_target_id = id;
            if (built) {
                render_target_descriptor_set->bind_texture(1, iter->second->image, sampler);
            }
            return;
        }
        MCH_CORE_ERROR("RenderTarget {} doesn't exist.")
    }
    
    void RenderTargetPool::clearRenderTarget(RenderTargetID id) {
        if (auto iter = render_targets.find(id); iter != render_targets.end()) {
            vk::ImageSubresourceRange range {};
            range.aspectMask = vk::ImageAspectFlagBits::eColor,
            range.baseArrayLayer = 0;
            range.layerCount = 1;
            range.baseMipLevel = 0;
            range.levelCount = 1;
            auto buffer = global_runtime_context->window_system->getAPIManager()->command_pool->allocate_single_use();
            buffer.clearColorImage(iter->second->image->image->image, iter->second->image->get_image_layout(), iter->second->clear_value, range);
            global_runtime_context->window_system->getAPIManager()->command_pool->free_single_use(buffer);
            return;
        }
        MCH_CORE_ERROR("RenderTarget {} doesn't exist.")
    }
    
    void RenderTargetPool::build() {
        if (built) {
            return;
        }
        built = true;
        render_target_descriptor_set->add_descriptors({
            { Match::ShaderStage::eFragment, 0, Match::DescriptorType::eTexture },
            { Match::ShaderStage::eFragment, 1, Match::DescriptorType::eStorageImage, static_cast<uint32_t>(render_targets.size()) },
        }).allocate();

        if (exist_render_targets.empty()) {
            render_target_descriptor_set->bind_texture(0, empty_texture, sampler);
            return;
        }
        render_target_descriptor_set->bind_texture(0, render_targets.at(output_render_target_id)->image, sampler);
        std::vector<std::shared_ptr<Match::StorageImage>> storage_images;
        for (auto render_target_id : exist_render_targets) {
            storage_images.push_back(render_targets.at(render_target_id)->image);
        }
        render_target_descriptor_set->bind_storage_images(1, storage_images);
    }
}
