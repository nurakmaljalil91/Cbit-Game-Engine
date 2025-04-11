/**
 * @file    MeshScene.cpp
 * @brief   Implementation file for the MeshScene class.
 * @details This file contains the implementation of the MeshScene class which is responsible for rendering a mesh.
 *          The MeshScene class is a subclass of the Scene class and is used
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#include "MeshScene.h"

MeshScene::MeshScene() {

}


MeshScene::~MeshScene() {

}

void MeshScene::setup() {
    Scene::setup();
    if (_mesh.loadOBJ("resources/models/bowling_pin.obj")) {
        LOG_INFO("OBJ file loaded successfully");
    } else {
        LOG_ERROR("Failed to load OBJ file");
    }
}

void MeshScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
}

void MeshScene::render() {
    Scene::render();
    _mesh.draw();
}

