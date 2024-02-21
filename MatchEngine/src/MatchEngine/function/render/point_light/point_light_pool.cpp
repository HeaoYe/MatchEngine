#include <MatchEngine/function/render/point_light/point_light_pool.hpp>

namespace MatchEngine {
    PointLightPool::PointLightPool(uint32_t max_point_light_count) : current_point_light_count(0) {
        point_light_buffer = std::make_shared<Match::Buffer>(sizeof(PointLight) * max_point_light_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
        point_light_buffer_ptr = static_cast<PointLight *>(point_light_buffer->map());
    }

    PointLightID PointLightPool::createPointLight(const PointLight &point_light) {
        *point_light_buffer_ptr = point_light;
        point_light_buffer_ptr ++;
        current_point_light_count ++;
        return current_point_light_count - 1;
    }

    PointLight *PointLightPool::getPointLightPtr(PointLightID id) {
        return static_cast<PointLight *>(point_light_buffer->map()) + id;
    }

    void PointLightPool::clear() {
        point_light_buffer_ptr = static_cast<PointLight *>(point_light_buffer->map());
        current_point_light_count = 0;
        memset(point_light_buffer_ptr, 0, point_light_buffer->size);
    }
}
