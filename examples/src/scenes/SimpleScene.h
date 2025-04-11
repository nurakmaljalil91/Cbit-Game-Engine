/**
 * @file    SimpleScene.h
 * @brief   Header file for the SimpleScene class.
 * @details SimpleScene class is a basic scene implementation for the Cbit Engine.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#ifndef SIMPLESCENE_H
#define SIMPLESCENE_H

#include "../../../src/core/GameObject.h"
#include "../../../src/core/Scene.h"

class SimpleScene : public Scene {
public:
    SimpleScene();

    ~SimpleScene() override;

    void setup() override;

    void update(float deltaTime, Input &input) override;

    void render() override;
private:
    std::string _uuid;
    GameObject _gameObject;
};


#endif //SIMPLESCENE_H
