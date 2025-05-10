/**
 * @file   EntityManager.cpp
 * @brief  Implementation file for the EntityManager class.
 * @detail This file contains the implementation of the EntityManager class which is responsible for managing
 * @author Nur Akmal bin Jalil
 * @date   2025-05-10
 */

#include "EntityManager.h"
#include <iostream>

void EntityManager::ClearData() const
{
    for (auto &entity : entities)
    {
        entity->Destroy();
    }
}
void EntityManager::Update(const float deltaTime) const
{
    for (auto &entity : entities)
    {
        entity->Update(deltaTime);
    }
}
void EntityManager::Render(ShaderProgram *shader) const
{
    for (auto &entity : entities)
    {
        entity->Render(shader);
    }
}
bool EntityManager::HasNoEntities() const
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

unsigned int EntityManager::GetEntityCount() const
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
