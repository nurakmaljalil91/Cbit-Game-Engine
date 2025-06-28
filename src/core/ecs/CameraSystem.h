/**
 * @file    CameraSystem.h
 * @brief   CameraSystem class header file
 * @details This file contains the definition of the CameraSystem class which is responsible for managing camera-related functionalities in the game engine.
 * @author  Nur Akmal bin Jalil
 * @date    2025-06-28
 */

#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <entt/entt.hpp>
#include "../graphic/ShaderManager.h"

class CameraSystem {
public:
    explicit CameraSystem(entt::registry &registry);

    void updateViewport(int width, int height);

    void bindActiveCamera(const std::shared_ptr<ShaderProgram> &shader) const;

    [[nodiscard]] glm::vec3 getActiveCameraPosition() const;

    [[nodiscard]] glm::mat4 getActiveViewMatrix() const;

    [[nodiscard]] glm::mat4 getActiveProjectionMatrix() const;

    // these give you back the *exact* view/proj just used in bindActiveCamera()
    [[nodiscard]] glm::mat4 getLastViewMatrix() const;

    [[nodiscard]] glm::mat4 getLastProjectionMatrix() const;

private:
    entt::registry &_registry;
    int _width = 1;
    int _height = 1;

    // mutable so we can update in a const method
    mutable glm::mat4 _lastViewMatrix = glm::mat4(1.0f);
    mutable glm::mat4 _lastProjectionMatrix = glm::mat4(1.0f);
};


#endif //CAMERASYSTEM_H
