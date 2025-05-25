/**
 * @file    MeshScene.h
 * @brief   This file contains the declaration of the MeshScene class which is responsible for rendering a mesh.
 * @details This file contains the declaration of the MeshScene class which is responsible for rendering a mesh.
 *          The MeshScene class is a subclass of the Scene class and is used
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#ifndef CBIT_MESHSCENE_H
#define CBIT_MESHSCENE_H

#include "../../../src/core/MeshQuad.h"
#include "core/project/Scene.h"

class MeshScene : public Scene {
public:
    MeshScene();

    ~MeshScene() override;

    void setup() override;

    void update(float deltaTime, Input &input) override;

    void render() override;
private:
    MeshQuad _quad;
    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;

};


#endif //CBIT_MESHSCENE_H
