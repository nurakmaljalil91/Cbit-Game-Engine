#ifndef IMAGE2D_H
#define IMAGE2D_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Component.h"

class Image2D : public Component
{
private:
    SDL_Texture *texture; // texture to be render
    int width, height;    // texture width and height

public:
    int layer; // layer for texture to be render

    Image2D(const char *filename, SDL_Renderer *renderer); // default Constructor
    virtual ~Image2D();                                    // default Deconstructor

    void Start() override;
    void Handle_Events() override;
    void Update(float delta_time) override;
    void Render(SDL_Renderer *renderer) override;
    void Clear() override;
}; // class Image2D

#endif // IMAGE2D_H