/**
 * @file    CubeScene.h
 * @brief   Header file for the CubeScene class.
 * @details This file contains the declaration of the CubeScene class which is a subclass of the Scene class.
 *          The CubeScene class is responsible for rendering a simple cube in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#ifndef CBIT_CUBESCENE_H
#define CBIT_CUBESCENE_H

#include "core/project/Scene.h"
#include "../../../src/core/graphic/ShaderProgram.h"
#include "../../../src/core/camera/Camera.h"

class CubeScene final : public Scene {
public:
    CubeScene();

    ~CubeScene() override;

    void setup() override;

    void update(float deltaTime, Input &input) override;

    void render() override;

private:
    GLuint _vao;
    GLuint _vbo;
    ShaderProgram _shaderProgram;
    Camera _camera;

    float _lastX, _lastY;
    bool _firstMouse;
    float _rotationAngle;

    void processInput(float deltaTime, const Input &input);
};


#endif //CBIT_CUBESCENE_H
