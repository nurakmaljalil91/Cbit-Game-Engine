/**
 * @file   EntityManager.cpp
 * @brief  Implementation file for the EntityManager class.
 * @detail This file contains the implementation of the EntityManager class which is responsible for managings
 * @author Nur Akmal bin Jalil
 * @date   2025-05-10
 */

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include "Component.h"
#include <vector>

class EntityManager
{
public:
    void ClearData() const;
    void Update(float deltaTime) const;
    void Render(ShaderProgram* shader) const;
    bool HasNoEntities() const;
    Entity& AddEntity(const char* entityName);
    std::vector<Entity*> GetEntities() const;
    void ListAllEntities() const;
    unsigned int GetEntityCount() const;

private:
    std::vector<Entity*> entities;
}; // class EntityManager

#endif //ENTITYMANAGER_H
