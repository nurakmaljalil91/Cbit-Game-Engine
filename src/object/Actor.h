#ifndef ACTOR_H
#define ACTOR_H

#include "../cthead.h"
#include "../math/Math.h"
#include "../graphic/Shader.h"
#include "../graphic/Texture.h"

namespace ct
{
class Actor
{
private:
    // class members
    // gameObject members
    const char *name;
    const char *tag;
    const char *id;
    bool is_active;

    // Transform members
    Vector2 position;
    Vector2 anchor;
    int width;
    int height;
    float scale;
    float rotation;
    Matrix4 world_transform;
    bool recomputed_world_transform;

    // Sprite member
    class Texture *texture;
    int texture_width;
    int texture_height;
    int layer;

public:
    // Constuctor & deconstructor
    Actor(class Game *_game);
    ~Actor();

    // main functions
    void Start();
    void Handle_Events();
    void Update(float delta_time);
    void Render(Shader *shader);

    // GameObject functions (Setter and Getter)
    void Set_Name(const char *mName);
    void Set_Tag(const char *mTag);
    void Set_ID(const char *mId);
    void Set_Active(bool mIs_active);

    const char *Get_Name();
    const char *Get_Tag();
    const char *Get_ID();
    bool Get_Active();

    // Transform functions Setter
    void Set_Position(const Vector2 &mPosition);
    void Set_Anchor(const Vector2 &mAnchor);
    void Set_Width(int mWidth);
    void Set_Height(int mHeight);
    void Set_Scale(float mScale);
    void Set_Rotation(float _rotation);

    // Transform functions Getter
    Vector2 Get_Position() const;
    Vector2 Get_Anchor() const;
    int Get_Width() const;
    int Get_Height() const;
    float Get_Scale() const;
    float Get_Rotation() const;

    void Compute_World_Transform();
    const Matrix4 &Get_World_Transform() const;

    Vector2 Get_Forward() const;
    // Sprite function

    void Set_Texture(Texture *_texture);
    void Set_Layer(int mLayer);

    Texture *Get_Texture();
    int Get_Layer();

}; // class Actor
} // namespace ct

#endif // ACTOR_H