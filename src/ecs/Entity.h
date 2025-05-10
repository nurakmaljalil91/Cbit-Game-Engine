/**
 * @file   Entity.cpp
 * @brief  Implementation file for the Entity class.
 * @detail This file contains the implementation of the Entity class which is responsible for managings
 * @author Nur Akmal bin Jalil
 * @date   2025-05-10
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.h"
#include "EntityManager.h"
#include "utilities/Math.h"

class Component;
class EntityManager;
// GameObject work as entity identifier
struct GameObject
{
    const char* name = ""; // name of the gameObject
    const char* tag = ""; // tag for gameObject to be found
    int layer = 0; // check where the layer of the entity
}; // struct GameObject

// Transform acts physical attributes saver
struct EntityTransform
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // position of the entity
    Quaternion rotation = Quaternion::Identity; // change rotation to Quaternion to support 3d rotation
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f); // scale for the entity
}; // struct Transform

class Entity
{
public:
    GameObject gameObject;
    EntityTransform transform;

    explicit Entity(const char* name);
    Entity(const EntityManager& entity_manager, const char* entity_name);

    // Entity(EntityManager &manager);
    // Entity(EntityManager &manager, const char *name);

    void HandleEvents() const;

    void Update(float deltaTime);

    void Render(ShaderProgram* shader) const;

    void Destroy();

    [[nodiscard]] bool IsActive() const;

    void ListAllComponents() const;

    [[nodiscard]] bool IsQueuedForRemoval() const;

    void QueueForRemoval();

    [[nodiscard]] glm::mat4 GetWorldPosition() const { return mWorldPosition; }

    template <typename T, typename... TArgs>
    T& AddComponent(TArgs&&... args)
    {
        T* newComponent(new T(std::forward<TArgs>(args)...));
        newComponent->owner = this;
        components.emplace_back(newComponent);
        componentTypeMap[&typeid(*newComponent)] = newComponent;
        newComponent->initialize();
        return *newComponent;
    }

    template <typename T>
    T* GetComponent()
    {
        return static_cast<T*>(componentTypeMap[&typeid(T)]);
    }

    template <typename T>
    [[nodiscard]] bool HasComponent() const
    {
        return componentTypeMap.count(&typeid(T));
    }

    void Clear(){}

private:
    // EntityManager &manager;
    bool isActive;
    glm::mat4 mWorldPosition;
    std::vector<Component*> components;
    std::map<const std::type_info*, Component*> componentTypeMap;

    bool mQueuedForRemoval;
};

#endif
