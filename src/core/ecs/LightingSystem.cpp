/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-06-28
 */

#include "LightingSystem.h"

#include "Components.h"
#include "core/graphic/Lighting.h"

LightingSystem::LightingSystem(entt::registry &registry): _registry(registry) {
}

void LightingSystem::applyAllLights(ShaderProgram &shader, const glm::vec3 &viewPos) {
    // Apply directional light
    for (auto entity: _registry.view<DirectionalLightComponent>()) {
        const auto &light = _registry.get<DirectionalLightComponent>(entity);
        DirectionalLight directionalLight{};
        directionalLight.direction = light.direction;
        directionalLight.color = light.color;
        directionalLight.ambient = light.ambient;
        Lighting::applyDirectionalLight(shader, directionalLight, viewPos);
    }

    // Apply point lights
    for (auto entity: _registry.view<PointLightComponent>()) {
        const auto &light = _registry.get<PointLightComponent>(entity);
        PointLight pointLight{};
        pointLight.position = light.position;
        pointLight.color = light.color;
        pointLight.constant = light.constant;
        pointLight.linear = light.linear;
        pointLight.quadratic = light.quadratic;
        Lighting::applyPointLight(shader, pointLight, viewPos);
    }

    // Apply spotlights
    for (auto entity: _registry.view<SpotLightComponent, CameraComponent>()) {
        const auto &light = _registry.get<SpotLightComponent>(entity);
        SpotLight spotLight{};
        spotLight.position = viewPos;
        spotLight.direction = light.direction;
        spotLight.color = light.color;
        spotLight.cutOff = glm::cos(glm::radians(light.cutOff));
        spotLight.outerCutOff = glm::cos(glm::radians(light.outerCutOff));
        Lighting::applySpotLight(shader, spotLight, viewPos);
    }
}
