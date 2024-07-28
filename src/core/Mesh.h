#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include <glad/glad.h>
#include "glm/glm.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
private:
    bool mLoaded;
    std::vector<Vertex> mVertices;
    GLuint mVBO, mVAO;

    void InitBuffers();

    // ====================== C++ game programming ==================//
    // Textures associated with this mesh
    std::vector<class Texture *> mTextures;
    // Vertex array associated with this mesh
    class VertexArray *mVertexArray;
    // Name of shader specified by mesh
    std::string mShaderName;
    // Stores object space bounding sphere radius
    float mRadius;
    // Specular power of surface
    float mSpecPower;

public:
    Mesh();

    ~Mesh();

    bool LoadOBJ(const std::string &filename);

    void Draw();

    // ====================== C++ game programming ==================//
    // Load/unload mesh
    bool LoadGPH(const std::string &fileName, class Application *renderer);

    void UnloadGPH();

    // Get the vertex array associated with this mesh
    class VertexArray *GetVertexArray() { return mVertexArray; }

    // Get a texture from specified index
    class Texture *GetTexture(size_t index);

    // Get name of shader
    const std::string &GetShaderName() const { return mShaderName; }
    // Get object space bounding sphere radius
    float GetRadius() const { return mRadius; }
    // Get specular power of mesh
    float GetSpecPower() const { return mSpecPower; }
};
#endif //MESH_H
