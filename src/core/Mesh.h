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
#include "Texture.h"

class Mesh {
public:
    Mesh();

    virtual ~Mesh();

    // Transform setters
    void setPosition(const glm::vec3 &pos) { position = pos; }
    void setSize(const glm::vec2 &sz) { size = sz; }
    void setScale(const glm::vec3 &scl) { scale = scl; }

    void setRotation(const float angleDegrees, const glm::vec3 &axisVec) {
        rotationAngle = angleDegrees;
        rotationAxis = axisVec;
    }

    void setRotation(const glm::vec3 &rot) { rotation = rot; }

    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    void setColor(const glm::vec4 &col) { color = col; }

    // Texture setters
    void setTexture(Texture *tex) {
        texture = tex;
        hasTexture = true;
    }

    void clearTexture() {
        texture = nullptr;
        hasTexture = false;
    }

    // Draw the mesh (builds model matrix, sets "model" uniform, binds VAO)
    void draw(const ShaderProgram &shader) const;

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
    glm::vec2 size = glm::vec2(0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f); // pitch, yaw, roll

    // Texture State
    Texture *texture = nullptr; // Texture ID
    bool hasTexture = false; // Whether the mesh has a texture
};


#endif //MESH_H
