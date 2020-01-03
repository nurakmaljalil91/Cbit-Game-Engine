#ifndef IMAGE2D_H
#define IMAGE2D_H

#include "Component.h"
#include "Entity.h"

class Image2D : public Component
{
private:
    Texture *texture;  // texture to be render
    int width, height; // texture width and height

public:
    int layer; // layer for texture to be render

    Image2D(Texture *_texture); // default Constructor
    virtual ~Image2D();        // default Deconstructor

    void Start() override;
    void Handle_Events() override;
    void Update(float delta_time) override;
    void Render(Shader *shader) override;
    void Clear() override;

    void Set_Texture(Texture *_texture); // Set the texture of the image
};                                      // class Image2D

#endif // IMAGE2D_H