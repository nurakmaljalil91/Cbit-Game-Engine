#ifndef ANIMATIONSPRITE_H
#define ANIMATIONSPRITE_H

#include <vector>
#include "Component.h"

class AnimationSprite : public Component
{
private:
    std::vector<SDL_Texture *> animations_texture;
    // SDL_Texture *texture;
    int width, height;
    SDL_Rect source_rect, destination_rect;
    float current_frame;
    float animation_fps;

public:
    AnimationSprite();
    virtual ~AnimationSprite();

    void Start() override;
    void Handle_Events() override;
    void Update(float delta_time) override;
    void Render(SDL_Renderer *renderer) override;
    void Clear() override;

};     // class AnimationSprite
#endif // ANIMATIONSPRITE_H