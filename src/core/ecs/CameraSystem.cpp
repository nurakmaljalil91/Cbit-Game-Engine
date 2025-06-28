/**
 * @file    CameraSystem.cpp
 * @brief   Implementation of the CameraSystem class
 * @details The CameraSystem class is responsible for managing camera-related functionalities in the game engine.
 * @author  Nur Akmal bin Jalil
 * @date    2025-06-28
 */

#include "CameraSystem.h"
#include "Components.h"

CameraSystem::CameraSystem(entt::registry &registry): _registry(registry) {
}

void CameraSystem::updateViewport(const int width, const int height) {
    _width = width;
    _height = height;
}

void CameraSystem::bindActiveCamera(const std::shared_ptr<ShaderProgram> &shader) const {
    for (const auto view = _registry.view<CameraComponent, TransformComponent>(); const auto entity: view) {
        auto &transform = view.get<TransformComponent>(entity);
        const auto &camera = view.get<CameraComponent>(entity);
        if (!camera.isPrimary) continue;

        // Update transform.position from orbital parameters:
        const float yawRadian = glm::radians(camera.yaw);
        const float pitchRadian = glm::radians(camera.pitch);
        glm::vec3 direction;
        direction.x = cos(yawRadian) * cos(pitchRadian);
        direction.y = sin(pitchRadian);
        direction.z = sin(yawRadian) * cos(pitchRadian);
        const glm::vec3 position = camera.target - direction * camera.distance;

        transform.position = position;

        transform.rotation.x = -camera.pitch; // pitch around X
        transform.rotation.y = -camera.yaw + 90.0f; // yaw around Y (adjust +90° if your forward axis differs)
        transform.rotation.z = 0.0f;

        // Build view matrix with lookAt:
        const glm::mat4 viewMatrix = glm::lookAt(position,
                                                 camera.target,
                                                 glm::vec3(0.0f, 1.0f, 0.0f));

        // Build projection as before:
        const float aspect = static_cast<float>(_width) / static_cast<float>(_height);
        const glm::mat4 projection = glm::perspective(glm::radians(camera.fov),
                                                      aspect,
                                                      camera.nearClip,
                                                      camera.farClip);

        _lastViewMatrix = viewMatrix;
        _lastProjectionMatrix = projection;
        // Set uniforms in shader
        shader->setMat4("view", viewMatrix);
        shader->setMat4("projection", projection);
        return;
    }
}

glm::vec3 CameraSystem::getActiveCameraPosition() const {
    for (const auto view = _registry.view<CameraComponent, TransformComponent>(); const auto entity: view) {
        const auto &transform = view.get<TransformComponent>(entity);
        if (const auto &camera = view.get<CameraComponent>(entity); camera.isPrimary) {
            return transform.position;
        }
    }
    return glm::vec3(0.0f); // Default position if no active camera found
}

glm::mat4 CameraSystem::getActiveViewMatrix() const {
    for (const auto view = _registry.view<CameraComponent, TransformComponent>(); const auto entity: view) {
        const auto &camera = view.get<CameraComponent>(entity);
        const auto &transform = view.get<TransformComponent>(entity);
        if (camera.isPrimary) {
            const glm::mat4 rotation = glm::toMat4(glm::quat(transform.rotation));
            const glm::mat4 translation = glm::translate(glm::mat4(1.0f), -transform.position);
            return rotation * translation;
        }
    }
    return {1.0f}; // Default view matrix if no active camera found
}

glm::mat4 CameraSystem::getActiveProjectionMatrix() const {
    for (const auto view = _registry.view<CameraComponent>(); const auto entity: view) {
        if (const auto &camera = view.get<CameraComponent>(entity); camera.isPrimary) {
            const float aspect = static_cast<float>(_width) / static_cast<float>(_height);
            return glm::perspective(glm::radians(camera.fov), aspect, camera.nearClip, camera.farClip);
        }
    }
    return {1.0f}; // Default projection matrix if no active camera found
}

glm::mat4 CameraSystem::getLastViewMatrix() const {
    return _lastViewMatrix;
}

glm::mat4 CameraSystem::getLastProjectionMatrix() const {
    return _lastProjectionMatrix;
}
