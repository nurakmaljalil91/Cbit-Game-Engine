#ifndef ENTITY_H
#define ENTITY_H

#include "../math/Math.h"

// GameObject work as entity identifier
struct GameObject
{
    const char *name = ""; // name of the gameObject
    const char *tag = "";  // tag for gameObject to be found
    int layer = 0;         // check where the layer of the entity
    bool active = true;    // only update and render when the entity is active
};                         // struct GameObject

// Transform act physical attributes saver
struct ETransform
{
    Vector3 position = Vector3(0.0f, 0.0f, 0.0f); // position of the entity
    Vector3 scale = Vector3(1.f, 1.f, 1.f);       // scale for the entity
    Quaternion rotation = Quaternion::Identity;   // change rotation to Quaternion to support 3d rotation
};                                                // struct Transform

class Entity
{
private:
public:
    Entity();          // Entity constructor
    virtual ~Entity(); // Entity deconstructor

}; // class Entity

#endif // ENTITY_H