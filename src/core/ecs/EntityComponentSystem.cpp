/**
 * @file    EntityComponentSystem.cpp
 * @brief   EntityComponentSystem class implementation file
 * @details This file contains the implementation of the EntityComponentSystem class.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#include "EntityComponentSystem.h"
#include <random>
#include "Components.h"
#include "GameObject.h"
#include "../locator/Locator.h"
#include "../../utilities/UUIDGenerator.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../graphic/Lighting.h"
#include "glm/gtx/string_cast.hpp"

EntityComponentSystem::EntityComponentSystem() = default;

EntityComponentSystem::~EntityComponentSystem() = default;

void EntityComponentSystem::update(float deltaTime) {
    // Update all game objects
}


void EntityComponentSystem::render(const CameraManager &cameraManager) {
    const auto &windowWidth = Locator::window()->getWidth();
    const auto &windowHeight = Locator::window()->getHeight();


    const std::shared_ptr<ShaderProgram> meshShader = Locator::shaders().get("mesh_lighting");
    meshShader->use();

    _cameraSystem.updateViewport(windowWidth, windowHeight);
    _cameraSystem.bindActiveCamera(meshShader);
    glm::vec3 cameraPosition = _cameraSystem.getActiveCameraPosition();

    _lightingSystem.applyAllLights(*meshShader, cameraPosition);


    // Spotlight (can disable with black color)
    // SpotLight spotLight;
    // spotLight.position = cameraPosition;
    // spotLight.direction = cameraPosition->getViewMatrix()[2]; // or custom dir
    // spotLight.color = glm::vec3(0.0f); // disable if unused
    // Lighting::applySpotLight(*meshShader, spotLight, editorCamera->getPosition());

    for (const auto quadView = _registry.view<QuadComponent, TransformComponent>();
         const auto entity: quadView
    ) {
        auto &transform = quadView.get<TransformComponent>(entity);
        auto &quad = quadView.get<QuadComponent>(entity);

        quad.mesh.setPosition(transform.position);
        quad.mesh.setRotation(transform.rotation);
        quad.mesh.setScale(transform.scale);

        if (_registry.any_of<TextureComponent>(entity)) {
            auto &texture = _registry.get<TextureComponent>(entity);
            quad.mesh.setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            quad.mesh.setTexture(&texture.texture);
        } else {
            quad.mesh.clearTexture();
        }

        quad.mesh.draw(*meshShader);
    }

    for (const auto cubeView = _registry.view<CubeComponent, TransformComponent>();
         const auto entity: cubeView
    ) {
        auto &transform = cubeView.get<TransformComponent>(entity);
        auto &cube = cubeView.get<CubeComponent>(entity);

        cube.mesh.setPosition(transform.position);
        cube.mesh.setRotation(transform.rotation);
        cube.mesh.setScale(transform.scale);

        if (_registry.any_of<TextureComponent>(entity)) {
            auto &texture = _registry.get<TextureComponent>(entity);
            // set color white
            cube.mesh.setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            cube.mesh.setTexture(&texture.texture);
        } else {
            cube.mesh.clearTexture();
        }

        cube.mesh.draw(*meshShader);
    }
}

void EntityComponentSystem::cleanup() {
    // clear all game objects
    _registry.clear();
}

GameObject EntityComponentSystem::createGameObject(const std::string &tag) {
    auto entity = GameObject(_registry.create(), this);
    entity.addComponent<TagComponent>(tag);
    entity.addComponent<IdComponent>(UUIDGenerator::generate());
    return entity;
}

void EntityComponentSystem::destroyGameObject(GameObject gameObject) {
    _registry.destroy(gameObject.getEntity());
}

GameObject EntityComponentSystem::getGameObject(const std::string &tag) {
    const auto view = _registry.view<TagComponent>();
    for (auto entity: view) {
        auto &tagComponent = view.get<TagComponent>(entity);
        if (tagComponent.tag == tag) {
            return {entity, this};
        }
    }
    return {entt::null, this};
}
