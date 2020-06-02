#include "Entity.h"

Entity::Entity() : recomputeWorldTransform(true) {}

Entity::~Entity() {}

void Entity::HandleEvents()
{
    if (gameObject.active)
    {
        for (auto &c : components)
        {
            c->HandleEvents();
        }
    }
}

void Entity::Update(float deltaTime)
{
    if (gameObject.active)
    {
        for (auto &c : components)
        {
            // update the transform for the component
            // c->Update_Transform(transform.position, transform.rotation, transform.scale);
            ComputeWorldTransform();
            c->Update(deltaTime);
        }
    }
}

void Entity::Render(Shader *shader)
{
    if (gameObject.active)
    {
        for (auto &c : components)
        {
            c->Render(shader);
        }
    }
}

void Entity::Clear()
{
    for (auto &c : components)
    {
        c->Clear();
    }
}

void Entity::ComputeWorldTransform()
{
    if (!transform.position.Is_Equal(mInitialPosition))
    {
        // std::cout << "Change needed" << std::endl;
        recomputeWorldTransform = true;
    }
    if (transform.scale != mInitialScale)
    {
        recomputeWorldTransform = true;
    }
    if (transform.rotation.Is_Equal(mInitialRotation)) // NOTE: if something wrong with rotation change this
    {
        recomputeWorldTransform = true;
    }
    if (recomputeWorldTransform)
    {
        recomputeWorldTransform = false;
        worldTransform = Matrix4::CreateScale(transform.scale);
        worldTransform *= Matrix4::CreateFromQuaternion(transform.rotation); // NOTE: Change for 3d rotation
        worldTransform *= Matrix4::CreateTranslation(transform.position);
        // std::cout << "This is running" << std::endl;
        //  for (auto &c : components)
        // {
        //     c->On_Update_World_Transform();
        // }
    }
    mInitialPosition = transform.position;
    mInitialScale = transform.scale;
    mInitialRotation = transform.rotation;
}