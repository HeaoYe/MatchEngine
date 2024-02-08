#include <MatchEngine/function/game_framework/scene_manager.hpp>
#include <MatchEngine/core/logger/logger.hpp>
#include "internal.hpp"

namespace MatchEngine {
    SceneManager::SceneManager() {
        initializeRuntimeSystem();

        state = RuntimeSystem::State::eInitialized;
    }

    bool SceneManager::hasRegisteredScene(const std::string &name) {
        return scenes.find(name) != scenes.end();
    }

    PointerWrapper<Game::Scene> SceneManager::createScene(const std::string &name) {
        if (scenes.find(name) != scenes.end()) {
            MCH_CORE_ERROR("Scene {} has been created.", name)
            return nullptr;
        }
        auto *scene = new Game::Scene(name);
        scenes.insert(std::make_pair(name, scene));
        if (active_scene == nullptr) {
            setActiveScene(scene);
        }
        return scene;
    }

    void SceneManager::loadScene(const std::string &name) {
        if (auto iter = scenes.find(name); iter != scenes.end()) {
            change_scene = iter->second;
            return;
        }
        MCH_CORE_ERROR("No scene named {}.", name);
    }
    
    MeshID SceneManager::loadMesh(const std::string &filename) {
        MeshRawData data {};
        static auto factory = global_runtime_context->window_system->getAPIManager()->create_resource_factory("thirdparty/Match/examples/Scene/resource");
        auto model = factory->load_model(filename);
        for (auto &vertex : model->vertices) {
            data.positions.push_back(vertex.pos);
            data.normals.push_back(vertex.normal);
            data.tex_coords.push_back({ 0, 0 });
            data.colors.push_back(vertex.color);
        }
        for (auto [name, mesh] : model->meshes) {
            for (auto &index : mesh->indices) {
                data.indices.push_back(index);
            }
        }
        return global_runtime_context->render_system->getActiveSceneRenderer()->getMeshPool()->uploadMeshRawData(&data, 1024);
    }

    void SceneManager::start() {
        active_scene->start();
        global_runtime_context->render_system->postActiveSceneStart();
    }
    
    void SceneManager::fixedTick() {
        active_scene->fixedTick();
    }
    
    void SceneManager::tick(float dt) {
        active_scene->tick(dt);
        active_scene->postTick(dt);
    }
    
    void SceneManager::swap() {
        if (change_scene == nullptr) {
            return;
        }
        setActiveScene(change_scene);
        change_scene = nullptr;
        start();
    }
    
    void SceneManager::setActiveScene(Game::Scene *scene) {
        if (active_scene != nullptr) {
            global_runtime_context->render_system->destoryActiveSceneRenderer();
        }
        active_scene = scene;
        global_runtime_context->render_system->createActiveSceneRenderer();
    }
    
    SceneManager::~SceneManager() {
        destoryRuntimeSystem();
        state = RuntimeSystem::State::eExited;

        for (auto [name, scene] : scenes) {
            delete scene;
        }
        scenes.clear();
    }
}
