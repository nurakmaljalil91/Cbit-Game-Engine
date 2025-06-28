/**
 * @file    LightingSystem.cpp
 * @brief   Implementation file for the LightingSystem class
 * @details The LightingSystem class is responsible for managing and applying lighting effects in the scene.
 * @author  Nur Akmal bin Jalil
 * @date    2025-06-28
 */

#include "LightingSystem.h"
#include "Components.h"
#include "core/graphic/Lighting.h"

LightingSystem::LightingSystem(entt::registry &registry): _registry(registry) {
}

void LightingSystem::applyAllLights(ShaderProgram &shader, const glm::vec3 &cameraPosition) const {
    // Apply directional light
    for (auto entity: _registry.view<DirectionalLightComponent>()) {
        const auto &directionalLightComponent = _registry.get<DirectionalLightComponent>(entity);
        DirectionalLight directionalLight{};
        directionalLight.direction = directionalLightComponent.direction;
        directionalLight.color = directionalLightComponent.color;
        directionalLight.ambient = directionalLightComponent.ambient;
        Lighting::applyDirectionalLight(shader, directionalLight, cameraPosition);
    }

    // Apply point lights
    for (auto entity: _registry.view<PointLightComponent>()) {
        const auto &pointLightComponent = _registry.get<PointLightComponent>(entity);
        PointLight pointLight{};
        pointLight.position = pointLightComponent.position;
        pointLight.color = pointLightComponent.color;
        pointLight.constant = pointLightComponent.constant;
        pointLight.linear = pointLightComponent.linear;
        pointLight.quadratic = pointLightComponent.quadratic;
        Lighting::applyPointLight(shader, pointLight, cameraPosition);
    }

    // Apply spotlights
    for (auto entity: _registry.view<SpotLightComponent, CameraComponent>()) {
        const auto &spotLightComponent = _registry.get<SpotLightComponent>(entity);
        SpotLight spotLight{};
        spotLight.position = cameraPosition;
        spotLight.direction = spotLightComponent.direction;
        spotLight.color = spotLightComponent.color;
        spotLight.cutOff = glm::cos(glm::radians(spotLightComponent.cutOff));
        spotLight.outerCutOff = glm::cos(glm::radians(spotLightComponent.outerCutOff));
        Lighting::applySpotLight(shader, spotLight, cameraPosition);
    }
}
