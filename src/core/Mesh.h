/**
 * @file    Mesh.h
 * @brief   This file contains the declaration of the Mesh class which is responsible for rendering a mesh.
 * @details This file contains the declaration of the Mesh class which is responsible for rendering a mesh.
 *          The Mesh class is a subclass of the Scene class and is used
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#ifndef CBIT_MESH_H
#define CBIT_MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../utilities/Logger.h"
#include <rapidjson/document.h>
#include <sstream>
#include <fstream>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    Mesh();

    ~Mesh();

    bool loadOBJ(const std::string &filename);

    void loadJSON(const std::string &jsonString);

    void bind() const;

    void unbind() const;

    void draw() const;

private:
    void _setupMesh();

    bool _loaded;
    std::vector<Vertex> _vertices;
    std::vector<glm::vec2> _texCoords;
    std::vector<glm::vec3> _normals;
    std::vector<GLuint> _indices;

    GLuint _VAO, _VBO, _EBO;
};


#endif //CBIT_MESH_H
