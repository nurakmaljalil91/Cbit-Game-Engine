#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
// #include "EntityManager.h"
#include "Component.h"
// #include "Constants.h"

// class EntityManager;
class Component;

// GameObject work as entity identifier
struct GameObject {
    const char *name = ""; // name of the gameObject
    const char *tag = ""; // tag for gameObject to be found
    int layer = 0; // check where the layer of the entity
}; // struct GameObject

// Transform act physical attributes saver
struct EntityTransform {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // position of the entity
    // Quaternion rotation = Quaternion::Identity;       // change rotation to Quaternion to support 3d rotation
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f); // scale for the entity
}; // struct Transform

class Entity {
private:
    // EntityManager &manager;
    bool isActive;
    glm::mat4 mWorldPosition;
    std::vector<Component *> components;
    std::map<const std::type_info *, Component *> componentTypeMap;

    bool mQueuedForRemoval;

public:
    GameObject gameObject;
    EntityTransform transform;

    Entity(const char *name);

    // Entity(EntityManager &manager);
    // Entity(EntityManager &manager, const char *name);

    void HandleEvents();

    void Update(float deltaTime);

    void Render(ShaderProgram *shader);

    void Destroy();

    bool IsActive() const;

    void ListAllComponents() const;

    bool IsQueuedForRemoval();

    void QueueForRemoval();

    glm::mat4 GetWorldPosition() { return mWorldPosition; }

    template<typename T, typename... TArgs>
    T &AddComponent(TArgs &&... args) {
        T *newComponent(new T(std::forward<TArgs>(args)...));
        newComponent->owner = this;
        components.emplace_back(newComponent);
        componentTypeMap[&typeid(*newComponent)] = newComponent;
        newComponent->initialize();
        return *newComponent;
    }

    template<typename T>
    T *GetComponent() {
        return static_cast<T *>(componentTypeMap[&typeid(T)]);
    }

    template<typename T>
    bool HasComponent() const {
        return componentTypeMap.count(&typeid(T));
    }
};

#endif
