/**
 * @file    TriangleScene.h
 * @brief   Header file for the TriangleScene class.
 * @details This file contains the definition of the TriangleScene class which is a subclass of the Scene class.
 *          The TriangleScene class is responsible for rendering a triangle on the screen.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-31
 */

#ifndef CBIT_TRIANGLESCENE_H
#define CBIT_TRIANGLESCENE_H

#include "core/project/Scene.h"
#include "../../src/core/ShaderProgram.h"
#include "../../src/core/VertexArray.h"

class TriangleScene : public Scene
{
public:
    TriangleScene();

    ~TriangleScene() override;

    void setup() override;

    void update(float deltaTime, Input& input) override;

    void render() override;

private:
    VertexArray _vertexArray;
    ShaderProgram _shaderProgram;
};

#endif //CBIT_TRIANGLESCENE_H
