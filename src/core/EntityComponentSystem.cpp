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
#include "../utilities/UUIDGenerator.h"
#include "glm/gtc/type_ptr.hpp"

EntityComponentSystem::EntityComponentSystem() {
    if (!_colorShader.loadShader(
        "resources/shaders/color.vert",
        "resources/shaders/color.frag")) {
        LOG_ERROR("Failed to load color shader");
    }
}

EntityComponentSystem::~EntityComponentSystem() = default;

void EntityComponentSystem::update(float deltaTime) {
    // Update all game objects
}

void EntityComponentSystem::render() {
    _colorShader.use();
    GLint colorLoc = glGetUniformLocation(
        _colorShader.getProgramID(), "u_Color"
    );
    // Render all game objects
    auto quadView = _registry.view<QuadComponent, TransformComponent>();

    for (auto entity: quadView) {
        auto &transform = quadView.get<TransformComponent>(entity);
        auto &quad = quadView.get<QuadComponent>(entity);

        quad.mesh.setCenter(transform.position.x, transform.position.y, transform.scale.x * 1, transform.scale.y * 1);

        // quad.mesh.setColor(quad.color);/ upload the tint color
        glUniform4fv(colorLoc, 1, glm::value_ptr(quad.color));


        quad.mesh.draw();
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
