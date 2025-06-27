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

    glm::mat4 getActiveViewMatrix() const;
    glm::mat4 getActiveProjectionMatrix() const;

private:
    entt::registry &_registry;
    int _width = 1;
    int _height = 1;
};


#endif //CAMERASYSTEM_H
