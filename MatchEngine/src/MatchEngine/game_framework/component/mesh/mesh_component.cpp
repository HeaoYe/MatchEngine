#include <MatchEngine/game_framework/component/mesh/mesh_component.hpp>
#include <MatchEngine/game_framework/component/transform/transform_component.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    void MeshComponent::onStart() {
        auto mesh_instance_pool = global_runtime_context->render_system->getActiveSceneRenderer()->getSwapData()->getMeshInstancePool();
        auto transform_component = master->queryComponent<TransformComponent>();
        if (transform_component != nullptr) {
            mesh_instance_pool->createMeshInstance({
                .location = transform_component->location,
                .rotation = transform_component->rotation,
                .scale = transform_component->scale,
                .mesh_id = mesh_id,
            });
        } else {
            mesh_instance_pool->createMeshInstance({
                .location = { 0, 0, 0 },
                .rotation = { 0, 0, 0 },
                .scale = { 1, 1, 1 },
                .mesh_id = mesh_id,
            });
        }
    }
}
