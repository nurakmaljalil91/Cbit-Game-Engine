#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "../math/Math.h"
#include "Component.h"
#include "Shader.h"

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
    bool active = true;    // only update and render when the entity is active
};                         // struct GameObject

// Transform act physical attributes saver
struct ETransform
{
    Vector3 position = Vector3(0.0f, 0.0f, 0.0f); // position of the entity
    float scale = 1.f;                            // scale for the entity
    Quaternion rotation = Quaternion::Identity;   // change rotation to Quaternion to support 3d rotation
};                                                // struct Transform

class Entity
{
private:
    bool mReadyToRemove;                                // check if entity is ready to remove
    std::vector<std::unique_ptr<Component>> components; // all components attach to the entity
    Vector3 mInitialPosition;                           // use for check if there is changes in position
    float mInitialScale;                              // use for check if there is changes in scale
    Quaternion mInitialRotation;                        // use for check if there is changes in rotation

public:
    ComponentArray component_array;
    ComponentBitSet component_bitset;
    GroupBitSet group_bitset;

    GameObject gameObject;        // GameObject of the entity
    ETransform transform;         // Transform of the entity
    Matrix4 worldTransform;       // The world transform
    bool recomputeWorldTransform; //only compute if need

    Entity();          // Entity constructor
    virtual ~Entity(); // Entity deconstructor

    void HandleEvents();          // handle the events from keyboard and mouse
    void Update(float deltaTime); // Update the process
    void Render(Shader *shader);  // Render or draw the entities
    void Clear();

    void ComputeWorldTransform();
    const Matrix4 &GetWorldTransform() const { return worldTransform; }

    template <typename T, typename... TArgs> // Add component to the entity
    T &AddComponent(TArgs &&... mArgs)
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
    T &GetComponent() const // Get the component
    {
        auto ptr(component_array[Get_Component_Type_ID<T>()]);
        return *static_cast<T *>(ptr);
    }

}; // class Entity

#endif // ENTITY_H