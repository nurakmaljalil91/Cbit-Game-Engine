#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Component.h"

class SpriteRenderer : public Component
{
private:
    SDL_Texture *texture;
    int width, height; // texture width and height
    SDL_Rect source_rect;

public:
    
    SpriteRenderer(const char *filename, SDL_Renderer *renderer,int _x, int _y, int _w, int _h);
    virtual ~SpriteRenderer();

    void Start() override;
    void Handle_Events() override;
    void Update(float delta_time) override;
    void Render(SDL_Renderer *renderer) override;
    void Clear() override;

    void Set_Sprite_Rect(int _x, int _y, int _w, int _h);
}; // class SpriteRenderer

#endif // SPRITERENDERER_H