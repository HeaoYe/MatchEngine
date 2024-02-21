#pragma once

#include <MatchEngine/core/base/runtime_system.hpp>
#include <MatchEngine/core/base/pointer_wrapper.hpp>
#include <MatchEngine/function/asset/mesh/mesh_pool.hpp>

namespace MatchEngine {
    // 资产系统
    class AssetSystem : public RuntimeSystem {
        DECLARE_RUNTIME_SYSTEM(AssetSystem)
    public:
        // 设置搜索资产的根文件夹
        void setRootDir(const std::string &path);

        // 从.obj文件加载一个Mesh
        MeshID loadMesh(const std::string &filename, const std::vector<std::string> &lods = {});

        size_t getMaxPrimitiveCount() const { return 4096; }
        size_t getMaxMeshCount() const { return 1024; }
        PointerWrapper<MeshPool> getMeshPool() { return mesh_pool.get(); }
    private:
        std::string path;
        std::unique_ptr<MeshPool> mesh_pool;
    };
}
