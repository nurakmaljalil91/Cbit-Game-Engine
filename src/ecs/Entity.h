#ifndef ENTITY_H
#define ENTITY_H

#include "../cthead.h"

namespace ct
{

class Entity;
class Component;

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

class GameObject
{
private:
public:
    bool is_active;
    const char *name;
    const char *tag;
    GameObject();
    virtual ~GameObject();
    void Set_Active(bool is_active);
}; // Class GameObject

class Entity
{
private:
public:
    class GameObject gameObject;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray component_array;
    ComponentBitSet component_bitset;
    GroupBitSet group_bitset;

    Entity();
    virtual ~Entity();

    void Start();
    void Handle_Events();
    void Update();
    void Render();

    bool Has_Group(Group mGroup);
    void Add_Group(Group mGroup);
    void Delete_Group(Group mGroup);

    template <typename T>
    bool Has_Component() const;
    template <typename T, typename... TArgs>
    T &Add_Component(TArgs &&... mArgs);
    template <typename T>
    T &Get_Component() const;

}; // Class GameObject
} // namespace ct

#endif // ENTITY_H