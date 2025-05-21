/**
 * @file    CubeTextureScene.h
 * @brief   Header file for the CubeTextureScene class.
 * @details This file contains the definition of the CubeTextureScene class which is responsible
 *          for rendering a cube with a texture.The CubeTextureScene class is a subclass of the
 *          Scene class and is used to render a cube with a texture.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#ifndef CBIT_CUBETEXTURESCENE_H
#define CBIT_CUBETEXTURESCENE_H

#include "core/project/Scene.h"
#include "../../../src/core/ShaderProgram.h"
#include "../../../src/core/camera/Camera.h"
#include "../../../src/core/Texture.h"
#include "../../../src/core/VertexArray.h"

class CubeTextureScene final : public Scene {
public:
    CubeTextureScene();

    ~CubeTextureScene() override;

    void setup() override;

    void update(float deltaTime, Input &input) override;

    void render() override;

private:
    GLuint _vao;
    GLuint _vbo;
    ShaderProgram _shaderProgram;
    Camera _camera;
    Texture _texture;
    VertexArray _vertexArray;

    float _lastX, _lastY;
    bool _firstMouse;
    float _rotationAngle;

    void processInput(float deltaTime, Input &input);
};


#endif //CBIT_CUBETEXTURESCENE_H
