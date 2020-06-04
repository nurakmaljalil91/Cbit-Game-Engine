#ifndef COMPONENT2_H
#define COMPONENT2_H

#include "Shader.h"

class Entity; // forward declaration of the Entity

class Component2
{
public:
    Entity *entity; // entity where the this component is gonna attach
    bool enabled;   // only update and render when enable

    Component2();          // Base constructor
    virtual ~Component2(); // Base deconstructor // make it polymorphic

    virtual void Start() = 0;                  // virtual function of start // = 0 no need to declare
    virtual void HandleEvents() = 0;           // virtual function of Handle Events
    virtual void Update(float delta_time) = 0; // virtual function of update
    virtual void Render(Shader *shader) = 0;   // virtual function of render
    virtual void Clear() = 0;                  // virtual function of  clear

}; // class Component

#endif // COMPONENT_H