#ifndef COMPONENT_H
#define COMPONENT_H

#include "../cthead.h"

namespace ct
{

class Entity;
class Component
{
private:
public:
    class Entity* entity;
    Component();
    ~Component();
    virtual void Start();
    virtual void Handle_Events();
    virtual void Update();
    virtual void Render();

}; // class component
} // namespace ct

#endif // COMPONENT_H