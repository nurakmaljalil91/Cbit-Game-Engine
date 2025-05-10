/**
 * @file   Entity.cpp
 * @brief  Implementation file for the Entity class.
 * @detail This file contains the implementation of the Entity class which is responsible for managing
 * @author Nur Akmal bin Jalil
 * @date   2025-05-10
 */

#include <iostream>
#include "Entity.h"

Entity::Entity(const char* name) :
    mQueuedForRemoval(false),
    isActive(true),
    mWorldPosition()
{
    this->gameObject.name = name;
}

Entity::Entity(const EntityManager& entity_manager, const char* entity_name): isActive(false), mWorldPosition(),
                                                                              mQueuedForRemoval(false)
{
}

// Entity::Entity(EntityManager &manager) : manager(manager), mQueuedForRemoval(false)
// {
//     this->isActive = true;
// }

// Entity::Entity(EntityManager &manager, const char *name) : manager(manager), mQueuedForRemoval(false)
// {
//     this->isActive = true;
//     this->gameObject.name = name;
// }

void Entity::HandleEvents() const
{
    if (isActive)
    {
        for (auto& c : components)
        {
            c->HandleEvents();
        }
    }
}

void Entity::Update(const float deltaTime)
{
    mWorldPosition = glm::mat4(1.0);
    mWorldPosition = glm::translate(glm::mat4(), transform.position) * glm::scale(glm::mat4(), transform.scale);
    if (isActive)
    {
        for (const auto& component : components)
        {
            component->Update(deltaTime);
        }
    }
}

void Entity::Render(ShaderProgram* shader) const
{
    if (isActive)
    {
        for (auto& component : components)
        {
            component->Render(shader);
        }
    }
}

void Entity::Destroy()
{
    this->isActive = false;
}

bool Entity::IsActive() const
{
    return this->isActive;
}

void Entity::ListAllComponents() const
{
    for (auto mapElement : componentTypeMap)
    {
        std::cout << "    Component<" << mapElement.first->name() << ">" << std::endl;
    }
}

bool Entity::IsQueuedForRemoval() const
{
    return mQueuedForRemoval;
}

void Entity::QueueForRemoval()
{
    mQueuedForRemoval = true;
}
