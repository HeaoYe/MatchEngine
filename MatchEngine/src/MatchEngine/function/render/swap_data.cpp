#include <MatchEngine/function/render/swap_data.hpp>
#include <internal.hpp>

namespace MatchEngine {
    SwapData::SwapData() {
        mesh_instance_pool = std::make_unique<MeshInstancePool>(global_runtime_context->render_system->getMaxMeshInstanceCount());
        point_light_pool = std::make_unique<PointLightPool>(global_runtime_context->render_system->getMaxPointLightCount());
    }

    void SwapData::clear() {
        mesh_instance_pool->clear();
        point_light_pool->clear();
    }

    SwapData::~SwapData() {
        mesh_instance_pool.reset();
        point_light_pool.reset();
    }
}
