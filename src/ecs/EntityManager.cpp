#include "EntityManager.h"
#include <iostream>

void EntityManager::ClearData()
{
    for (auto &entity : entities)
    {
        entity->Destroy();
    }
}
void EntityManager::Update(float deltaTime)
{
    for (auto &entity : entities)
    {
        entity->Update(deltaTime);
    }
}
void EntityManager::Render(ShaderProgram *shader)
{
    for (auto &entity : entities)
    {
        entity->Render(shader);
        // std::cout << "R In\n";
    }
    // std::cout << "R Out\n";
}
bool EntityManager::HasNoEntities()
{
    return entities.size() == 0;
}

Entity &EntityManager::AddEntity(const char *entityName)
{
    Entity *entity = new Entity(*this, entityName);
    entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity *> EntityManager::GetEntities() const
{
    return entities;
}

unsigned int EntityManager::GetEntityCount()
{
    return entities.size();
}

void EntityManager::ListAllEntities() const
{
    unsigned int i = 0;
    for (auto &entity : entities)
    {
        std::cout << "Entity [" << i << "]" << entity->gameObject.name << std::endl;
        entity->ListAllComponents();
        i++;
    }
}
