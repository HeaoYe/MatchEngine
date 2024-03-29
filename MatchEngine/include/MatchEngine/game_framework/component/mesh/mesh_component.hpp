#pragma once

#include <MatchEngine/game_framework/component/component.hpp>
#include <MatchEngine/function/asset/mesh/mesh_id.hpp>

namespace MatchEngine::Game {
    // Mesh组件
    REFLECT_CLASS(NAME = "MeshComponent")
    class MeshComponent : public Component {
        DECLARE_COMPONENT_CUSTOM_CONSTRUCTION(MeshComponent)
    private:
        void onCreate() override;
    public:
        MeshComponent(MeshID mesh_id) : mesh_id(mesh_id) {}
        MeshID mesh_id;
    };
}
