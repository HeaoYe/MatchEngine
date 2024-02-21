#pragma once

#include <MatchEngine/core/base/macro.hpp>
#include <MatchEngine/function/render/point_light/point_light.hpp>
#include <MatchEngine/function/render/point_light/point_light_id.hpp>
#include <Match/vulkan/resource/buffer.hpp>

namespace MatchEngine {
    // 点光源池, 管理所有点光源
    class PointLightPool {
        NoCopyMoveConstruction(PointLightPool)
    public:
        PointLightPool(uint32_t max_point_light_count);

        PointLightID createPointLight(const PointLight &point_light);
        PointLight *getPointLightPtr(PointLightID id);
        void clear();
    INNER_VISIBLE:
        uint32_t current_point_light_count;
        std::shared_ptr<Match::Buffer> point_light_buffer;
        PointLight *point_light_buffer_ptr;
    };
}
