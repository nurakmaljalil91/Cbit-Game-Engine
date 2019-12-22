#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"

namespace ct
{

class Entity;
class Component
{
private:
public:
    class Entity *entity;
    Component();
    ~Component();
    virtual void Start() = 0;
    virtual void Handle_Events() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;

}; // class component
} // namespace ct

#endif // COMPONENT_H