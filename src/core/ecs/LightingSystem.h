/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-06-28
 */
#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include <entt/entt.hpp>
#include "core/graphic/ShaderProgram.h"

class LightingSystem {
public:
    LightingSystem(entt::registry &registry);

    void applyAllLights(ShaderProgram &shader, const glm::vec3 &viewPos);

private:
    entt::registry &_registry;
};

#endif //LIGHTINGSYSTEM_H
