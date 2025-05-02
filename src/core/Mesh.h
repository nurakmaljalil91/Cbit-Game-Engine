/**
 * @file    Mesh.h
 * @brief   Base class for drawable meshes
 * @details This file contains the definition of the Mesh class which is used to represent a 3D mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */

#ifndef MESH_H
#define MESH_H

#include "ShaderProgram.h"

class Mesh {
public:
    Mesh();

    virtual ~Mesh();

    // Transform setters
    void setPosition(const glm::vec3 &pos) { position = pos; }
    void setScale(const glm::vec3 &scl) { scale = scl; }

    void setRotation(float angleDegrees, const glm::vec3 &axisVec) {
        rotationAngle = angleDegrees;
        rotationAxis = axisVec;
    }

    // Draw the mesh (builds model matrix, sets "model" uniform, binds VAO)
    void draw(ShaderProgram &shader);

protected:
    // Must be implemented by derived classes to upload vertex/index data
    virtual void setupMesh() = 0;

    // GPU handles
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    // Number of indices to draw
    GLuint indexCount;

    // Local transform state
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
};


#endif //MESH_H
