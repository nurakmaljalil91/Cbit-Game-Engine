#ifndef ACTOR_H
#define ACTOR_H

#include "../cthead.h"
#include "../math/Math.h"

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

    // Sprite member
    SDL_Texture *texture;
    int layer;

public:
    // Constuctor & deconstructor
    Actor();
    ~Actor();

    // main functions
    void Start();
    void Handle_Events();
    void Update(float delta_time);
    void Render(SDL_Renderer *renderer);

    // GameObject functions (Setter and Getter)
    void Set_Name(const char *mName);
    void Set_Tag(const char *mTag);
    void Set_ID(const char *mId);
    void Set_Active(bool mIs_active);

    const char *Get_Name();
    const char *Get_Tag();
    const char *Get_ID();
    bool Get_Active();

    //Transform functions
    void Set_Position(Vector2 mPosition);
    void Set_Anchor(Vector2 mAnchor);
    void Set_Width(int mWidth);
    void Set_Height(int mHeight);
    void Set_Scale(float mScale);

    Vector2 Get_Position();
    Vector2 Get_Anchor();
    int Get_Width();
    int Get_Height();
    float Get_Scale();

    // Sprite function
    SDL_Texture *Load_Texture(const char *filename, SDL_Renderer *renderer);
    void Set_sprite(SDL_Texture *mTexture);
    void Set_Layer(int mLayer);

    SDL_Texture *Get_sprite();
    int Get_Layer();

}; // class Actor
} // namespace ct

#endif // ACTOR_H