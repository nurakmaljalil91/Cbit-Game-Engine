#include "Actor.h"
#include "../Game.h"


//class Actor
ct::Actor::Actor(Game *game) : name(nullptr),
                               tag(nullptr),
                               id(nullptr),
                               is_active(true),
                               position(Vector2(0, 0)),
                               anchor(Vector2(0, 0)),
                               width(32),
                               height(32),
                               scale(1),
                               rotation(0),
                               texture(nullptr),
                               layer(0)
{
}
ct::Actor::~Actor() {}

//main function
void ct::Actor::Start() {}
void ct::Actor::Handle_Events()
{
    if (this->is_active)
    {
        // Handle events here
    }
}
void ct::Actor::Update(float delta_time)
{
    if (this->is_active)
    {
        // update here
        Compute_World_Transform();
    }
}
void ct::Actor::Render(Shader *shader)
{
    if (this->is_active)
    {
        // render here
        if (texture)
        {
            Matrix4 scale_material = Matrix4::CreateScale(
                static_cast<float>(texture_width),
                static_cast<float>(texture_height),
                1.0f);

            Matrix4 world = scale_material * this->Get_World_Transform();

            shader->Set_Matrix_Uniform("world_transform",world);

            texture->SetActive();

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }
    }
}

// GameObject functions (Setter and Getter)
void ct::Actor::Set_Name(const char *mName)
{
    name = mName;
}
void ct::Actor::Set_Tag(const char *mTag)
{
    tag = mTag;
}
void ct::Actor::Set_ID(const char *mId)
{
    id = mId;
}
void ct::Actor::Set_Active(bool mIs_active)
{
    is_active = mIs_active;
}

const char *ct::Actor::Get_Name()
{
    return this->name;
}
const char *ct::Actor::Get_Tag()
{
    return this->tag;
}
const char *ct::Actor::Get_ID()
{
    return id;
}
bool ct::Actor::Get_Active()
{
    return is_active;
}

//Transform functions
void ct::Actor::Set_Position(const Vector2 &mPosition)
{
    position = mPosition;
    recomputed_world_transform = true;
}
void ct::Actor::Set_Anchor(const Vector2 &mAnchor)
{
    anchor = mAnchor;
    recomputed_world_transform = true;
}

void ct::Actor::Set_Width(int mWidth)
{
    width = mWidth;
}
void ct::Actor::Set_Height(int mHeight)
{
    height = mHeight;
}
void ct::Actor::Set_Scale(float mScale)
{
    scale = mScale;
    recomputed_world_transform = true;
}

void ct::Actor::Set_Rotation(float _rotation)
{
    rotation = _rotation;
    recomputed_world_transform = true;
}

Vector2 ct::Actor::Get_Position() const
{
    return position;
}

Vector2 ct::Actor::Get_Anchor() const
{
    return anchor;
}

int ct::Actor::Get_Width() const
{
    return width;
}

int ct::Actor::Get_Height() const
{
    return height;
}

float ct::Actor::Get_Scale() const
{
    return scale;
}

float ct::Actor::Get_Rotation() const
{
    return rotation;
}

void ct::Actor::Compute_World_Transform()
{
    if (recomputed_world_transform)
    {
        recomputed_world_transform = false;
        // Scale, rotatem and teanslate
        world_transform = Matrix4::CreateScale(scale);
        world_transform *= Matrix4::CreateRotationZ(rotation);
        world_transform *= Matrix4::CreateTranslation(Vector3(position.x, position.y, 0.0f));
    }
}

const Matrix4 &ct::Actor::Get_World_Transform() const
{
    return world_transform;
}

void ct::Actor::Set_Layer(int mLayer)
{
    layer = mLayer;
}

Vector2 ct::Actor::Get_Forward() const
{
    return Vector2(Math::Cos(rotation), Math::Sin(rotation));
}

void ct::Actor::Set_Texture(Texture *_texture)
{
    texture = _texture;
    texture_width = texture->GetWidth();
    texture_height = texture->GetHeight();
}

Texture *ct::Actor::Get_Texture()
{
    return texture;
}
int ct::Actor::Get_Layer()
{
    return layer;
}
