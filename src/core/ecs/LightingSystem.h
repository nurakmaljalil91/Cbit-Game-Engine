/**
 * @file    LightingSystem.h
 * @brief   Header file for the LightingSystem class
 * @details The LightingSystem class is responsible for managing and applying lighting effects in the scene.
 * @author  Nur Akmal bin Jalil
 * @date    2025-06-28
 */
#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include <entt/entt.hpp>
#include "core/graphic/ShaderProgram.h"

class LightingSystem {
public:
    explicit LightingSystem(entt::registry &registry);

    void applyAllLights(ShaderProgram &shader, const glm::vec3 &cameraPosition) const;

private:
    entt::registry &_registry;
};

#endif //LIGHTINGSYSTEM_H
