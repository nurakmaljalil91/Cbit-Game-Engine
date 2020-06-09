#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include "../glm/glm.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh
{
private:
    bool mLoaded;
    std::vector<Vertex> mVertices;
    GLuint mVBO, mVAO;

    void InitBuffers();

public:
    Mesh();
    ~Mesh();

    bool LoadOBJ(const std::string &filename);
    void Draw();
};
#endif //MESH_H