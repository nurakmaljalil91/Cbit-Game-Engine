/**
 * @file   EntitiesManager.h
 * @brief  Header file for the EntitiesManager class.
 * @detail This file contains the definition of the EntitiesManager class which is responsible for managing
 * @author Nur Akmal bin Jalil
 * @date   2025-05-10
 */

#ifndef ENTITIESMANAGER_H
#define ENTITIESMANAGER_H

#include <memory>
#include <vector>
#include "Entity.h"

class EntitiesManager
{
public:
    void Add(const std::shared_ptr<Entity>& entity); // Add the entity to entities
    void Add(std::vector<std::shared_ptr<Entity>>& otherEntity); // Add the entity to entities

    void Start(); // Start all the entities' initialization
    void HandleEvents() const; // Handle events all the entities
    void Update(float delta_time); // Update all the entities' update
    void Render(ShaderProgram* shader) const; // Render all the entities render
    void Clear() const; // Clear all teh entities clear

    bool Have(const std::shared_ptr<Entity>& entity); // Check if entity inside the manager
    [[nodiscard]] std::vector<std::shared_ptr<Entity>> GetEntities() const;

    void InitNewEntities(); // Process new entities
    void QueueForRemoval(); // Process to remove entities

    void ImGuiShowEntities(bool* p_open);

private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> newEntities;
    std::vector<int> layer_orders;
}; // class EntitiesManager

#endif // ENTITIESMANAGER_H
