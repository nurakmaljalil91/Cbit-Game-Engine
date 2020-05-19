#ifndef TEXT_H
#define TEXT_H

#include "Component.h"

class Text : public Component
{
private:
public:
    Text(); // default Constructor
    virtual ~Text();

    void Start() override;
    void Handle_Events() override;
    void Update(float delta_time) override;
    void Render(SDL_Renderer *renderer) override;
    void Clear() override;

};     // class Text
#endif // TEXT_H