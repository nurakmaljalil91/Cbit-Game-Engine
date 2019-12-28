#ifndef ANIMATIONSPRITE_H
#define ANIMATIONSPRITE_H

#include "Component.h"

class AnimationSprite : public Component
{
private:
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