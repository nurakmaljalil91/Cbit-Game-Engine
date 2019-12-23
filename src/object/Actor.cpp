#include "Actor.h"

//class Actor
ct::Actor::Actor() : name(nullptr),
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
    }
}
void ct::Actor::Render(SDL_Renderer *renderer)
{
    if (this->is_active)
    {
        // render here
        if (texture)
        {
            SDL_Rect rect;
            rect.w = static_cast<int>(width * scale);
            rect.h = static_cast<int>(height * scale);

            rect.x = static_cast<int>(position.x - rect.w / 2);
            rect.y = static_cast<int>(position.y - rect.h / 2);

            SDL_RenderCopyEx(renderer,
                             texture,
                             nullptr,
                             &rect,
                             -Math::ToDegrees(rotation),
                             nullptr,
                             SDL_FLIP_NONE);
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
void ct::Actor::Set_Position(Vector2 mPosition)
{
    position = mPosition;
}
void ct::Actor::Set_Anchor(Vector2 mAnchor)
{
    anchor = mAnchor;
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
}

Vector2 ct::Actor::Get_Position()
{
    return position;
}
Vector2 ct::Actor::Get_Anchor()
{
    return anchor;
}
int ct::Actor::Get_Width()
{
    return width;
}
int ct::Actor::Get_Height()
{
    return height;
}
float ct::Actor::Get_Scale()
{
    return scale;
}

// Sprite function
SDL_Texture *ct::Actor::Load_Texture(const char *filename, SDL_Renderer *renderer)
{
    SDL_Texture *tex = nullptr;
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface)
    {
        SDL_Log("Failed to load texture file %s", filename);
        return nullptr;
    }
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex)
    {
        SDL_Log("Failed to convert surface to texture for %s", filename);
        return nullptr;
    }
    return tex;
}
void ct::Actor::Set_sprite(SDL_Texture *mTexture)
{
    texture = mTexture;
    //SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
}
void ct::Actor::Set_Layer(int mLayer)
{
    layer = mLayer;
}

SDL_Texture *ct::Actor::Get_sprite()
{
    return texture;
}
int ct::Actor::Get_Layer()
{
    return layer;
}
