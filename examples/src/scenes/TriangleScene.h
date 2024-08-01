/**
 * @file TriangleScene.h
 * @brief Header file for the TriangleScene class.
 *
 * This file contains the definition of the TriangleScene class which is a subclass of the Scene class.
 * The TriangleScene class is responsible for rendering a triangle on the screen.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-31
 */

#ifndef CBIT_TRIANGLESCENE_H
#define CBIT_TRIANGLESCENE_H

#include "../../src/core/Scene.h"
#include "../../src/core/ShaderProgram.h"

class TriangleScene : public Scene {
public:
    ~TriangleScene() override;

    void setup() override;

    void update(float deltaTime, Input &input) override;

    void render() override;

private:
    GLuint _vao;
    GLuint _vbo;
    ShaderProgram _shaderProgram;

    const std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";
};

#endif //CBIT_TRIANGLESCENE_H
