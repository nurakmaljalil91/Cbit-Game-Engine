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

#include "entt/entt.hpp"

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

private:
    entt::registry _registry;
    friend class GameObject;

};



#endif //ENTITYCOMPONENTSYSTEM_H
