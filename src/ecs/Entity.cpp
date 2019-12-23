#include "Entity.h"

ct::GameObject::GameObject() : is_active(true),
                               name("no name"),
                               tag("no tag") {}
ct::GameObject::~GameObject() {}

void ct::GameObject::Set_Active(bool p_is_active)
{
    is_active = p_is_active;
}

ct::Entity::Entity()
{
   
}
ct::Entity::~Entity() {}

void ct::Entity::Start() {

}
void ct::Entity::Handle_Events()
{
    if (gameObject.is_active)
    {
        for (auto &c : components)
            c->Handle_Events();
    }
}
void ct::Entity::Update()
{
    if (gameObject.is_active)
    {
        for (auto &c : components)
            c->Update();
    }
}
void ct::Entity::Render()
{

    if (gameObject.is_active)
    {
        for (auto &c : components)
            c->Render();
    }
}

bool ct::Entity::Has_Group(Group mGroup)
{
    return group_bitset[mGroup];
}

void ct::Entity::Add_Group(Group mGroup) {}
void ct::Entity::Delete_Group(Group mGroup)
{
    group_bitset[mGroup] = false;
}

template <typename T>
bool ct::Entity::Has_Component() const
{
    return component_bitset[Get_Component_Type_ID<T>()];
}
template <typename T, typename... TArgs>
T &ct::Entity::Add_Component(TArgs &&... mArgs)
{
    T *c(new T(std::forward<TArgs>(mArgs)...));
    c->entity = this;
    std::unique_ptr<Component> uPtr{c};

    component_array[Get_Component_Type_ID<T>()] = c;
    component_bitset[Get_Component_Type_ID<T>()] = true;

    c->Start();
    return *c;
}

template <typename T>
T &ct::Entity::Get_Component() const
{
    auto ptr(component_array[Get_Component_Type_ID<T>()]);
    return *static_cast<T *>(ptr);
}