/**
 * @file    RectangleCameraScene.h
 * @brief   Header file for the RectangleCamera class
 * @details This file contains the definition of the RectangleCamera class which is responsible for
 *          managing the camera in the game. The RectangleCamera class is responsible for setting up
 *          the camera and updating the view matrix.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-02
 */

#ifndef CBIT_RECTANGLECAMERASCENE_H
#define CBIT_RECTANGLECAMERASCENE_H

#include "../../src/core/Scene.h"
#include "../../../src/core/ShaderProgram.h"
#include "../../../src/core/camera/Camera.h"

class RectangleCameraScene : public Scene {
public:

    RectangleCameraScene();

    ~RectangleCameraScene() override;

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

    void processInput(float deltaTime, Input &input);
};




#endif //CBIT_RECTANGLECAMERASCENE_H
