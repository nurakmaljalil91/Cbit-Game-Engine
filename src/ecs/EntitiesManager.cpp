#include "EntitiesManager.h"

void EntitiesManager::Add(std::shared_ptr<Entity> entity)
{
    newEntities.push_back(entity); // add entity to the entities
}

void EntitiesManager::Add(std::vector<std::shared_ptr<Entity>> &otherEntity)
{
    newEntities.insert(newEntities.end(), otherEntity.begin(), otherEntity.end());
}

void EntitiesManager::Start() {}

void EntitiesManager::HandleEvents()
{
    for (const auto &e : entities)
    {
        e->HandleEvents(); // Handle events here
    }
}

void EntitiesManager::Update(float delta_time)
{
    InitNewEntities();
    QueueForRemoval();
    for (const auto &e : entities)
    {
        e->Update(delta_time); // update all the entities
    }
}

void EntitiesManager::Render(ShaderProgram *shader)
{
    //std::cout << "Begin" << std::endl;
    // TODO: Draw based on entities layer order
    for (const auto &e : entities)
    {
        e->Render(shader); // Render all the entities
    }
}

void EntitiesManager::Clear()
{
    // for (const auto &e : entities)
    // {
    //     e->Clear(); // Clear all the entities
    // }
}

bool EntitiesManager::Have(std::shared_ptr<Entity> entity)
{
    if (std::find(entities.begin(), entities.end(), entity) != entities.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EntitiesManager::InitNewEntities()
{
    entities.insert(entities.end(), newEntities.begin(), newEntities.end()); // combine the new entities with the original entities

    newEntities.clear(); // clear the oject inside the new entities
}

void EntitiesManager::QueueForRemoval()
{
    auto entity_iter = entities.begin(); // create iterator for the entity

    while (entity_iter != entities.end())
    {

        auto ent = *entity_iter;

        if (ent->IsQueuedForRemoval())
        {
            entity_iter = entities.erase(entity_iter);
        }
        else
        {
            ++entity_iter;
        }
    }
}