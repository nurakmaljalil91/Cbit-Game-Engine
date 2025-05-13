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
#include "Locator.h"
#include "SDL_video.h"
#include "../utilities/UUIDGenerator.h"
#include "glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

EntityComponentSystem::EntityComponentSystem() {
}

EntityComponentSystem::~EntityComponentSystem() = default;

void EntityComponentSystem::update(float deltaTime) {
    // Update all game objects
}

void EntityComponentSystem::render(const glm::mat4 &view, const glm::mat4 &projection) {
    const std::shared_ptr<ShaderProgram> meshShader = Locator::shaders().get("mesh");
    meshShader->use();
    meshShader->setMat4("view", view);
    meshShader->setMat4("projection", projection);

    for (const auto quadView = _registry.view<QuadComponent, TransformComponent>();
         const auto entity: quadView
    ) {
        auto &transform = quadView.get<TransformComponent>(entity);
        auto &quad = quadView.get<QuadComponent>(entity);

        quad.mesh.setPosition(transform.position);
        quad.mesh.setRotation(transform.rotation);
        quad.mesh.setScale(transform.scale);

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

        cube.mesh.draw(*meshShader);
    }
}

void EntityComponentSystem::render(const CameraManager &cameraManager) {
    const auto &windowWidth = Locator::window()->getWidth();
    const auto &windowHeight = Locator::window()->getHeight();

    const auto *editorCamera = cameraManager.getCamera(CameraType::Editor);

    const glm::mat4 editorView = editorCamera->getViewMatrix();
    const glm::mat4 editorProjection = editorCamera->getProjectionMatrix(
        static_cast<float>(windowWidth) / static_cast<float>(windowHeight));

    const std::shared_ptr<ShaderProgram> meshShader = Locator::shaders().get("mesh");
    meshShader->use();
    meshShader->setMat4("view", editorView);
    meshShader->setMat4("projection", editorProjection);

    for (const auto quadView = _registry.view<QuadComponent, TransformComponent>();
         const auto entity: quadView
    ) {
        auto &transform = quadView.get<TransformComponent>(entity);
        auto &quad = quadView.get<QuadComponent>(entity);

        quad.mesh.setPosition(transform.position);
        quad.mesh.setRotation(transform.rotation);
        quad.mesh.setScale(transform.scale);

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
