#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Renderer.h"
#include "VertexArray.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include "rapidjson/document.h"
#include "SDL2/SDL_log.h"
#include "../math/Math.h"

class Mesh
{
private:
    std::vector<Texture *> textures; // textures associated with this mesh
    VertexArray *vertex_array;       // vertex array associated with this mesh
    std::string shader_name;         // name of shade specified with this mesh
    float radius;                    // store object space bounding sphere radius
    float specular_power;            // specular power of surface
public:
    Mesh();  // Constructor
    ~Mesh(); // Deconstructor

    bool Load(const std::string &filename, Renderer *renderer); // Load Mesh
    void Unload();                                              // Unload the mesh

    VertexArray *Get_Vertex_Array() { return vertex_array; }    // Get the vertex array
    Texture *Get_Texture(size_t index);                         // Get the texture from the specific index
    const std::string &Get_Shader_Name();                       // Get the shader name
    float Get_Radius() const { return radius; }                 // Get the radius
    float Get_Specular_Power() const { return specular_power; } // Get the specular power of the mesh
};                                                              // class Mesh

#endif // MESH_H