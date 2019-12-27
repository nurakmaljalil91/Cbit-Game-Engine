#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const char *filename, SDL_Renderer *renderer)
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
        SDL_Rect rect;
        rect.w = static_cast<int>(width * scale.x);
        rect.h = static_cast<int>(height * scale.y);

        rect.x = static_cast<int>(position.x - rect.w / 2);
        rect.y = static_cast<int>(position.y - rect.h / 2);

        SDL_RenderCopyEx(
            renderer,                     // render target to draw
            texture,                      // texture to draw
            nullptr,                      // source rectangle
            &rect,                        //destination rectangle
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