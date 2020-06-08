#ifndef COMPONENT_H
#define COMPONENT_H

#include "../core/ShaderProgram.h"

class Entity;

class Component
{
public:
    Entity *owner;
    virtual ~Component() {}
    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Render(ShaderProgram *shader) {}
};

#endif