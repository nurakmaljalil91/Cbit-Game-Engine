#include <iostream>
#include "Entity.h"

Entity::Entity(const char *name) : mQueuedForRemoval(false),
                                   isActive(true)
{

    this->gameObject.name = name;
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

void Entity::HandleEvents()
{
    if (isActive)
    {
        for (auto &c : components)
        {
            c->HandleEvents();
        }
    }
}
void Entity::Update(float deltaTime)
{
    mWorldPosition = glm::mat4(1.0);
    mWorldPosition = glm::translate(glm::mat4(), transform.position) * glm::scale(glm::mat4(), transform.scale);
    if (isActive)
    {
        for (auto &component : components)
        {
            component->Update(deltaTime);
        }
    }
}

void Entity::Render(ShaderProgram *shader)
{
    if (isActive)
    {
        for (auto &component : components)
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

bool Entity::IsQueuedForRemoval()
{
    return mQueuedForRemoval;
}

void Entity::QueueForRemoval()
{
    mQueuedForRemoval = true;
}