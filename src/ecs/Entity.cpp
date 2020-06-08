#include <iostream>
#include "Entity.h"

Entity::Entity(EntityManager &manager) : manager(manager)
{
    this->isActive = true;
}

Entity::Entity(EntityManager &manager, const char *name) : manager(manager)
{
    this->isActive = true;
    this->gameObject.name = name;
}

void Entity::Update(float deltaTime)
{
    mWorldPosition = glm::mat4(1.0);
    mWorldPosition = glm::translate(glm::mat4(), transform.position) * glm::scale(glm::mat4(), transform.scale);
    for (auto &component : components)
    {
        component->Update(deltaTime);
    }
}

void Entity::Render(ShaderProgram *shader)
{
    for (auto &component : components)
    {
        component->Render(shader);
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