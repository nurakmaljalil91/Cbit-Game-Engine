#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const char *filename, SDL_Renderer *renderer, int _x, int _y, int _w, int _h)
{
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface)
    {
        SDL_Log("[ERROR] Failed to load texture for %s", filename);
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr)
    {
        SDL_Log("[ERROR] Failed to convert surface to texture for %s", filename);
    }

    source_rect.x = _x;
    source_rect.y = _y;
    source_rect.w = _w;
    source_rect.h = _h;
}

SpriteRenderer::SpriteRenderer(SDL_Texture *_texture, SDL_Renderer *renderer, int _x, int _y, int _w, int _h)
{
    texture = _texture;

    if (texture == nullptr)
    {
        SDL_Log("[ERROR] Failed to get the texture ");
    }

    source_rect.x = _x;
    source_rect.y = _y;
    source_rect.w = _w;
    source_rect.h = _h;
}

SpriteRenderer::~SpriteRenderer() {}

void SpriteRenderer::Start() {}

void SpriteRenderer::Handle_Events() {}

void SpriteRenderer::Update(float delta_time) {}

void SpriteRenderer::Render(SDL_Renderer *renderer)
{
    if (texture)
    {
        SDL_QueryTexture(texture, NULL, NULL, &width, &height); // because we use single image this allow us to take width and height of the single texture
        SDL_Rect destination_rect;
        destination_rect.w = static_cast<int>(source_rect.w * scale.x);
        destination_rect.h = static_cast<int>(source_rect.h * scale.y);

        destination_rect.x = static_cast<int>(position.x - destination_rect.w / 2);
        destination_rect.y = static_cast<int>(position.y - destination_rect.h / 2);

        SDL_RenderCopyEx(
            renderer,                     // render target to draw
            texture,                      // texture to draw
            &source_rect,                 // source rectangle
            &destination_rect,            //destination rectangle
            -Math::ToDegrees(rotation.x), // Rotation angle (in degree, clockwise)
            nullptr,                      // Point to rotate about
            SDL_FLIP_NONE                 // How to flip texture
        );
    }
}

void SpriteRenderer::Clear()
{
    SDL_DestroyTexture(texture);
}

void SpriteRenderer::Set_Sprite_Rect(int _x, int _y, int _w, int _h)
{
    source_rect.x = _x;
    source_rect.y = _y;
    source_rect.w = _w;
    source_rect.h = _h;
}