#include <MatchEngine/game_framework/component/mesh/mesh_component.hpp>
#include <MatchEngine/game_framework/component/transform/transform_component.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    void MeshComponent::onCreate() {
        auto mesh_instance_pool = global_runtime_context->render_system->getSwapData()->getMeshInstancePool();
        auto transform_component = master->queryComponent<TransformComponent>();
        if (transform_component != nullptr) {
            mesh_instance_pool->createMeshInstance({
                .location = transform_component->location,
                .rotation = transform_component->rotation,
                .scale = transform_component->scale,
                .mesh_id = mesh_id,
            }, master->getUUID());
            transform_component->registerMemberUpdateCallback([transform_component, uuid = master->getUUID()](Component *component) {
                auto ptr = global_runtime_context->render_system->getSwapData()->getMeshInstancePool()->getMeshInstancePtr(uuid);
                ptr->location = transform_component->location;
                ptr->rotation = transform_component->rotation;
                ptr->scale = transform_component->scale;
            });
        } else {
            mesh_instance_pool->createMeshInstance({
                .location = { 0, 0, 0 },
                .rotation = { 0, 0, 0 },
                .scale = { 1, 1, 1 },
                .mesh_id = mesh_id,
            }, master->getUUID());
        }
    }
}
