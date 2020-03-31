#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "SDL2/SDL.h"
#include "../math/Math.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "iostream"
//#include "../Game.h"
//#include "Mesh.h"

struct DirectionlLight
{
    Vector3 direction;      // Direction of the light
    Vector3 diffuse_color;  // Diffuse color
    Vector3 specular_color; // Specular color
};

class Renderer
{
private:
    std::unordered_map<std::string, Texture *> textures; // map of textures loaded
    //std::unordered_map<std::string, Mesh *> meshes;      // map of meshes loaded
    // std::vector<Sprite> sprites
    //std::vector<MeshComponent *> mesh_components;
    //ct::Game *game;
    Shader *sprite_shader;        // sprite shader
    VertexArray *sprite_vertices; // Sprite vertex array
    Shader *mesh_shader;

    // view / projection for 3D shaders
    Matrix4 view;
    Matrix4 projection;
    float screen_width;
    float screen_height;
    // Lighting data
    Vector3 ambient_light;
    DirectionlLight direction_light;

    // Window
    SDL_Window *window;    // window
    SDL_GLContext context; // OpenGl context

    // private function
    bool Load_Shaders();
    void Create_Sprite_Vertices();
    void Set_Light_Uniforms(Shader *_shader);

public:
    Renderer(); // Constructor
    ~Renderer(); // Deconstructor

    bool Initialize(float _screen_width, float _screen_height);
    void Shutdown();
    void Unload_Data();

    void Render();

    // FIXME: Add function sprite component
    void Add_Sprite();
    void Remove_Sprite();

    // FIXME:: Add function for mesh component

    Texture *Get_Texture(const std::string &filename);
    //Mesh *Get_Mesh(const std::string &filename);

    void Set_View_Matrix(const Matrix4 &_view) { view = _view; }
    void Set_Ambient_Light(const Vector3 &_ambient) { ambient_light = _ambient; }
    DirectionlLight &Get_Directional_Light() { return direction_light; }

    float Get_Screen_Width() const { return screen_width; }
    float Get_Screen_Height() const { return screen_height; }

}; // class Renderer

#endif // RENDERER_H