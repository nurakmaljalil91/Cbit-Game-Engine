#include "Image2D.h"

Image2D::Image2D(Texture *_texture)
{
    texture = _texture;

    if (texture)
    {
        width = texture->GetWidth();
        height = texture->GetHeight();
    }
    else
    {
        width = 0;
        height = 0;
        SDL_Log("[ERROR] Image2D constructor can't init texture");
    }
}

Image2D::~Image2D() {}

void Image2D::Start() {}

void Image2D::Handle_Events() {}

void Image2D::Update(float delta_time) {}

void Image2D::Render(Shader *shader)
{
    if (texture)
    {
        // Scale the quad by the width/height of texture
        Matrix4 scale_material = Matrix4::CreateScale(
            static_cast<float>(width),
            static_cast<float>(height),
            entity->transform.scale.x);
       
        Matrix4 world = scale_material * entity->Get_World_Transform();

        shader->Set_Matrix_Uniform("world_transform", world);
        texture->SetActive();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Image2D::Clear()
{
}

void Image2D::Set_Texture(Texture *_texture)
{
    texture = _texture;

    if (texture)
    {
        width = texture->GetWidth();
        height = texture->GetHeight();
    }
    else
    {
        width = 0;
        height = 0;
        SDL_Log("[ERROR] Image2D constructor can't set texture");
    }
}