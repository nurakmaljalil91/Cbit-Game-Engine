#ifndef COMPONENT_H
#define COMPONENT_H

class Entity; // forward declaration of the Entity

class Component
{
public:
    Entity *owner; // entity where the this component is gonna attach
    // bool enabled;  // only update and render when enable

    Component();          // Base constructor
    virtual ~Component(); // Base deconstructor // make it polymorphic

    virtual void Start() = 0;                  // virtual function of start // = 0 no need to declare
    virtual void HandleEvents() = 0;           // virtual function of Handle Events
    virtual void Update(float delta_time) = 0; // virtual function of update
    virtual void Render() = 0;                 // virtual function of render
    virtual void Clear() = 0;                  // virtual function of  clear

}; // class Component

#endif // COMPONENT_H