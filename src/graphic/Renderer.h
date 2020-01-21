#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "SDL2/SDL.h"
#include "../math/Math.h"

struct DirectionlLight
{
    Vector3 direction;      // Direction of the light
    Vector3 diffuse_color;  // Diffuse color
    Vector3 specular_color; // Specular color
};

class Renderer
{
public:
    Renderer(class Game *_game);
    ~Renderer();

    bool Initialize(float _screen_width, float _screen_height);
    void Shutdown();
    void Unload_Data();


    void Render();

    void Add_Sprite()
}

#endif // RENDERER_H