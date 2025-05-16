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

#include "CameraManager.h"
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

    void render(const glm::mat4 &view, const glm::mat4 &projection);

    void render(const CameraManager &cameraManager);

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

    template<typename Component>
    [[nodiscard]] auto &getComponent(const entt::entity entity) {
        return _registry.get<Component>(entity);
    }

    // add component to entity
    template<typename T, typename... Args>
    T &addComponent(const entt::entity entity, Args &&... args) {
        // Check if the component already exists
        if (hasComponent<T>(entity)) {
            LOG_WARN("Component already added");
            return _registry.get<T>(entity);
        }
        return _registry.emplace<T>(entity, std::forward<Args>(args)...);
    }

    [[nodiscard]] entt::registry &getRegistry() { return _registry; }

private:
    entt::registry _registry;
    friend class GameObject;

    void _renderComponentSystem(const glm::mat4 &view, const glm::mat4 &projection);
};


#endif //ENTITYCOMPONENTSYSTEM_H
