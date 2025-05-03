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

EntityComponentSystem::EntityComponentSystem() {
}

EntityComponentSystem::~EntityComponentSystem() = default;

void EntityComponentSystem::update(float deltaTime) {
    // Update all game objects
}

void EntityComponentSystem::render() {
    int screenWidth, screenHeight;

    SDL_GetWindowSize(Locator::window()->getSDLWindow(), &screenWidth, &screenHeight);

    glViewport(0, 0, screenWidth, screenHeight);

    // build an ortho that maps [0…W]×[0…H] → [−1…+1]×[−1…+1]
    const glm::mat4 projection =glm::ortho(
        0.0f, static_cast<float>(screenWidth),
        0.0f, static_cast<float>(screenHeight),
        -1.0f, 1.0f
    );



    constexpr auto view = glm::mat4(1.0f); // no camera

    const std::shared_ptr<ShaderProgram> meshShader = Locator::shaders().get("mesh");
    meshShader->use();
    meshShader->setMat4("view", view);
    meshShader->setMat4("projection", projection);
    // Render all game objects

    for (const auto quadView = _registry.view<QuadComponent, TransformComponent>();
         const auto entity: quadView
    ) {
        auto &transform = quadView.get<TransformComponent>(entity);
        auto &quad = quadView.get<QuadComponent>(entity);

        quad.mesh.setPosition(transform.position);
        quad.mesh.setRotation(0.0f, transform.position);
        quad.mesh.setSize({transform.scale.x, transform.scale.y});

        quad.mesh.draw(*meshShader);
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
