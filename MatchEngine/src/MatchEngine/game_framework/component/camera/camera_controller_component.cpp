#include <MatchEngine/game_framework/component/camera/camera_controller_component.hpp>
#include <MatchEngine/game_framework/game_object.hpp>
#include <glm/ext/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "internal.hpp"

namespace MatchEngine::Game {
    void CameraControllerComponent::onStart() {
        for (auto camera_component_class_name : { "CameraComponent", "OrthographicCameraComponent", "PerspectiveCameraComponent" }) {
            camera_component = static_cast<CameraComponent *>(master->queryComponent(camera_component_class_name));
            if (camera_component != nullptr) {
                break;
            }
        }
        if (camera_component == nullptr) {
            MCH_CORE_ERROR("Add Camera Controller Component Without Camera Component.")
            return;
        }

        transform_component = master->queryComponent<TransformComponent>();
        if (transform_component == nullptr) {
            location = new glm::vec3(0, 0, 0);
            yaw = new float(0);
            pitch = new float(0);
        } else {
            location = &transform_component->location;
            yaw = &transform_component->rotation.y;
            pitch = &transform_component->rotation.x;
            transform_component->registerMemberUpdateCallback([this](Component *) {
                updateViewMatrix();
            });
        }

        updateViewMatrix();
    }

    void CameraControllerComponent::onTick(float dt) {
        auto mouse_delta = static_cast<double>(sensitivity * dt) * global_runtime_context->input_system->getMouseDelta();
        bool changed = false;
        if (global_runtime_context->input_system->isMouseButtonPressed(MouseButton::eLeft)) {
            *yaw -= mouse_delta.x;
            *pitch = std::clamp<float>(*pitch + mouse_delta.y, -89.0f, 89.0f);
            changed = true;
        }
        auto delta = dt * speed;
        auto tangent = delta * glm::rotateY(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(*yaw));
        auto bitangent = glm::rotateY(tangent, glm::radians(90.0f));

        if (global_runtime_context->input_system->isKeyPressed(Key::eW)) {
            *location += tangent;
            changed = true;
        }
        if (global_runtime_context->input_system->isKeyPressed(Key::eS)) {
            *location -= tangent;
            changed = true;
        }
        if (global_runtime_context->input_system->isKeyPressed(Key::eA)) {
            *location += bitangent;
            changed = true;
        }
        if (global_runtime_context->input_system->isKeyPressed(Key::eD)) {
            *location -= bitangent;
            changed = true;
        }
        if (global_runtime_context->input_system->isKeyPressed(Key::eSpace)) {
            location->y += delta;
            changed = true;
        }
        if (global_runtime_context->input_system->isKeyPressed(Key::eLeftShift)) {
            location->y -= delta;
            changed = true;
        }
        if (changed) {
            updateViewMatrix();
        }
    }

    void CameraControllerComponent::updateViewMatrix() {
        auto view_direction = glm::rotateY(glm::rotateX(glm::vec3(0, 0, 1), glm::radians(*pitch)), glm::radians(*yaw));
        global_runtime_context->camera_system->reportCameraViewMatrix(camera_component->camera_id, glm::lookAt(*location, *location + view_direction, { 0, 1, 0 }));
    }

    CameraControllerComponent::~CameraControllerComponent() {
        if (transform_component == nullptr) {
            delete location;
            delete yaw;
            delete pitch;
        }
    }
}
