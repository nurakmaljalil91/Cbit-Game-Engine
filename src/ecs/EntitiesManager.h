#ifndef ENTITIESMANAGER_H
#define ENTITIESMANAGER_H

#include <memory>
#include <vector>
#include <algorithm>

#include "Entity.h"

class EntitiesManager {
private:
    std::vector<std::shared_ptr<Entity> > entities;
    std::vector<std::shared_ptr<Entity> > newEntities;
    //std::vector<int> layer_orders;

public:
    void Add(std::shared_ptr<Entity> entity); // Add the entity to entities
    void Add(std::vector<std::shared_ptr<Entity> > &otherEntity); // Add the entity to entities

    void Start(); // Start all the entities initialization
    void HandleEvents(); // Handle events all the entities
    void Update(float delta_time); // Update all the entities update
    void Render(ShaderProgram *shader); // Render all the entities render
    void Clear(); // Clear all teh entities clear

    bool Have(std::shared_ptr<Entity> entity); // Check if entity inside the manager
    std::vector<std::shared_ptr<Entity> > GetEntities() const;

    void InitNewEntities(); // Process new entities
    void QueueForRemoval(); // Process to remove entities

    void ImGuiShowEntities(bool *p_open);
}; // class EntitiesManager

#endif // ENTITIESMANAGER_H
