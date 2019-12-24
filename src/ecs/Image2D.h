#ifndef IMAGE2D_H
#define IMAGE2D_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Component.h"

class Image2D : public Component
{
private:
public:
    Image2D();          // default Constructor
    virtual ~Image2D(); // default Deconstructor

    void Start() override;
    void Handle_Events() override;
    void Update(float delta_time) override;
    void Render(SDL_Renderer *renderer) override;
    void Clear() override;
}; // class Image2D

#endif // IMAGE2D_H