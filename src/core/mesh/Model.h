/**
 * @file    Model.h
 * @brief   This file contains the declaration of the Model class which is responsible for managing 3D models.
 * @details This file contains the declaration of the Model class which is responsible for managing 3D models.
 *          The Model class provides functionality for loading, binding, and rendering 3D models.
@author  Nur Akmal bin Jalil
@date    2024-08-03
 */

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../../utilities/Logger.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Model {
public:
    Model();

    ~Model();

    bool loadOBJ(const std::string &filename);

    void loadJSON(const std::string &jsonString);

    void bind() const;

    void unbind() const;

    void draw() const;

private:
    void _setupModel();

    bool _loaded;
    std::vector<Vertex> _vertices;
    std::vector<glm::vec2> _texCoords;
    std::vector<glm::vec3> _normals;
    std::vector<GLuint> _indices;

    GLuint _VAO, _VBO, _EBO;
};


#endif //MODEL_H
