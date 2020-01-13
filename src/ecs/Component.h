#ifndef COMPONENT_H
#define COMPONENT_H

#include "../math/Math.h"
#include "SDL2/SDL.h"
#include "../graphic/Shader.h"
#include "../graphic/Texture.h"

//#include "Entity.h" // this will cause compiler error

class Entity; // forward declaration of the Entity

class Component
{
protected:
public:
    Entity *entity;       // entity where the this component is gonna attach
    Vector3 position;     // position of entity
    Vector3 rotation;     // rotation of entity
    Vector3 scale;        // scale of entity
    bool enabled;         // only update and render when enable
    Component();          // Base constructor
    virtual ~Component(); // Base deconstructor // make it polymorphic

    // void Update_Transform(Vector3 mPosition, Vector3 mRotation, Vector3 mScale); // update the current transform of the entity
    // virtual void On_Update_World_Transform();
    virtual void Start() = 0;                  // virtual function of start // = 0 no need to declare
    virtual void Handle_Events() = 0;          // virtual function of Handle Events
    virtual void Update(float delta_time) = 0; // virtual function of update
    virtual void Render(Shader *shader) = 0;   // virtual function of render
    virtual void Clear() = 0;                  // virtual function of  clear
};                                             // class component
#endif                                         // COMPONENT_H