#include <MatchEngine/game_framework/component/mesh/mesh_component.hpp>
#include <MatchEngine/game_framework/component/transform/transform_component.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    void MeshComponent::onStart() {
        auto mesh_pool = global_runtime_context->render_system->getActiveSceneRenderer()->getMeshPool();
        auto transform_component = master->queryComponent<TransformComponent>();
        if (transform_component != nullptr) {
            mesh_pool->addMeshInstance({
                .mesh_id = mesh_id,
                .location = transform_component->location,
                .rotation = transform_component->rotation,
                .scale = transform_component->scale,
            });
        } else {
            mesh_pool->addMeshInstance({
                .mesh_id = mesh_id,
                .location = { 0, 0, 0 },
                .rotation = { 0, 0, 0 },
                .scale = { 1, 1, 1 },
            });
        }
    }
}
