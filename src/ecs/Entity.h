#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "../math/Math.h"
#include "Component.h"
#include "SDL2/SDL.h"
#include "../graphic/Shader.h"

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID Get_New_Component_Type_ID()
{
    static ComponentID last_ID = 0u;
    return last_ID;
}

template <typename T>
inline ComponentID Get_Component_Type_ID() noexcept
{
    static_assert(std::is_base_of<Component, T>::value, "");
    static ComponentID type_ID = Get_New_Component_Type_ID();
    return type_ID;
}

constexpr std::size_t max_components = 32;
constexpr std::size_t max_groups = 32;

using ComponentBitSet = std::bitset<max_components>;
using GroupBitSet = std::bitset<max_groups>;

using ComponentArray = std::array<Component *, max_components>;

// GameObject work as entity identifier
struct GameObject
{
    const char *name = ""; // name of the gameObject
    const char *tag = "";  // tag for gameObject to be found
    int layer = 0;         // check where the layer of the entity
    bool is_active = true; // only update and render when the entity is active
};                         // struct GameObject

// Transform act physical attributes saver
struct ETransform
{
    Vector3 position = Vector3(0.0f, 0.0f, 0.0f); // position of the entity
    // Vector2 position_2d = Vector2(1.0f, 1.0f);    // 2d position of the entity
    // Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f); // rotation of the entity
    Quaternion rotation = Quaternion::Identity; // change rotation to Quaternion to support 3d rotation
    float scale = 1.0f;  // scale for the entity
};                       // struct Transform

class Entity
{
private:
    bool ready_to_remove;                               // check if entity is ready to remove
    std::vector<std::unique_ptr<Component>> components; // all components attach to the entity
    Vector3 initial_position;                           // use for check if there is changes in position
    Quaternion initial_rotation;                        // use for check if there is changes in rotation
    float initial_scale;                                // use for check if there is changes in scale

public:
    ComponentArray component_array;
    ComponentBitSet component_bitset;
    GroupBitSet group_bitset;

    GameObject gameObject;          // GameObject of the entity
    ETransform transform;           // Transform of the entity
    Matrix4 world_transform;        // The world transform
    bool recompute_world_transform; //only compute if need

    Entity();          // Entity constructor
    virtual ~Entity(); // Entity deconstructor // make it polymorphic

    bool Has_Group(Group mGroup) // FIXME: Not implement
    {
        return group_bitset[mGroup];
    }

    void Add_Group(Group mGroup); // FIXME: Not implement

    void Delete_Group(Group mGroup) // FIXME: Not implement
    {
        group_bitset[mGroup] = false;
    }

    template <typename T>
    bool Has_Component() const // Check if entity has component
    {
        return component_bitset[Get_Component_Type_ID<T>()];
    }

    template <typename T, typename... TArgs> // Add component to the entity
    T &Add_Component(TArgs &&... mArgs)
    {
        T *c(new T(std::forward<TArgs>(mArgs)...)); // Create the component
        c->entity = this;                           // initialize the component with the entity
        std::unique_ptr<Component> uPtr{c};         // NOTE:?
        components.emplace_back(std::move(uPtr));   // add the component to the list of components

        component_array[Get_Component_Type_ID<T>()] = c;
        component_bitset[Get_Component_Type_ID<T>()] = true;
        c->Start(); // Start or initialize the component
        return *c;
    }

    template <typename T>
    T &Get_Component() const // Get the component
    {
        auto ptr(component_array[Get_Component_Type_ID<T>()]);
        return *static_cast<T *>(ptr);
    }

    void Handle_Events()
    {
        if (gameObject.is_active)
        {
            for (auto &c : components)
            {
                c->Handle_Events();
            }
        }
    }

    void Update(float delta_time) // update all the component in the components
    {
        if (gameObject.is_active)
        {
            for (auto &c : components)
            {
                // update the transform for the component
                // c->Update_Transform(transform.position, transform.rotation, transform.scale);
                Compute_World_Transform();
                c->Update(delta_time);
            }
        }
    }

    void Render(Shader *shader) // Render all the component in the components
    {
        if (gameObject.is_active)
        {
            for (auto &c : components)
            {
                c->Render(shader);
            }
        }
    }

    void Clear() // Render all the component in the components
    {
        for (auto &c : components)
        {
            c->Clear();
        }
    }

    void Ready_To_Remove() // set the entity ready to be remove from entities manager
    {
        ready_to_remove = true;
    }

    bool Is_Ready_To_Remove() // Return the ready to move boolean for the entities manager
    {
        return ready_to_remove;
    }

    void Compute_World_Transform()
    {

        if (!transform.position.Is_Equal(initial_position))
        {
            // std::cout << "Change needed" << std::endl;
            recompute_world_transform = true;
        }
        if (transform.scale != initial_scale)
        {
            recompute_world_transform = true;
        }
        if (transform.rotation.Is_Equal(initial_rotation)) // NOTE: if something wrong with rotation change this
        {
            recompute_world_transform = true;
        }
        if (recompute_world_transform)
        {
            recompute_world_transform = false;
            world_transform = Matrix4::CreateScale(transform.scale);
            world_transform *= Matrix4::CreateFromQuaternion(transform.rotation); // NOTE: Change for 3d rotation
            world_transform *= Matrix4::CreateTranslation(transform.position);
            // std::cout << "This is running" << std::endl;
            //  for (auto &c : components)
            // {
            //     c->On_Update_World_Transform();
            // }
        }
        initial_position = transform.position;
        initial_scale = transform.scale;
        initial_rotation = transform.rotation;
    }

    const Matrix4 &Get_World_Transform() const { return world_transform; }
};     // Class Entity
#endif // ENTITY_H