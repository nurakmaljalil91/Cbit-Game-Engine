/**
 * @file    EntityComponentSystem.h
 * @brief   Header file for the EntityComponentSystem class
 * @details This file contains the definition of the EntityComponentSystem class.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#ifndef ENTITYCOMPONENTSYSTEM_H
#define ENTITYCOMPONENTSYSTEM_H

#include <string>

#include "Components.h"
#include "Locator.h"
#include "entt/entt.hpp"
#include "utilities/Logger.h"

class GameObject;

class EntityComponentSystem {
public:
    EntityComponentSystem();

    ~EntityComponentSystem();

    void update(float deltaTime);

    void render();

    void cleanup();

    GameObject createGameObject(const std::string &tag);

    void destroyGameObject(GameObject gameObject);

    GameObject getGameObject(const std::string &tag);

    template<typename... Components>
    auto getAllGameObjects() {
        return _registry.view<Components...>();
    }

    template<typename... Components>
    auto getGameObjectsWith() {
        return _registry.view<Components...>();
    }

    [[nodiscard]] bool validGameObject(const entt::entity entity) const {
        return _registry.valid(entity);
    }

    template<typename... Components>
    [[nodiscard]] bool hasComponent(const entt::entity entity) const {
        return _registry.all_of<Components...>(entity);
    }

    template<typename... Components>
    [[nodiscard]] auto getComponents(const entt::entity entity) {
        return _registry.get<Components...>(entity);
    }

    // add component to entity
    template<typename T, typename... Args>
    T &addComponent(const entt::entity entity, Args &&... args) {
        // Check if the component already exists
        if (hasComponent<T>(entity)) {
            LOG_WARN("Component already added");
            return _registry.get<T>(entity);
        }

        // If component is Transform component, set position center of the screen
        if constexpr (std::is_same_v<T, TransformComponent> && sizeof...(args) == 0) {
            auto &transform = _registry.emplace<T>(entity, std::forward<Args>(args)...);
            const auto windowWidth = Locator::window()->getWidth();
            const auto windowHeight = Locator::window()->getHeight();
            glViewport(0, 0, windowWidth, windowHeight);
            transform.position.y = windowHeight * 0.5f;
            transform.position.x = windowWidth * 0.5f;
            transform.scale.x = 200.0f;
            transform.scale.y = 200.0f;
            return transform;
        }

        return _registry.emplace<T>(entity, std::forward<Args>(args)...);
    }

private:
    entt::registry _registry;
    friend class GameObject;
};


#endif //ENTITYCOMPONENTSYSTEM_H
