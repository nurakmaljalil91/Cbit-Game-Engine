/**
 * @file    OrbitCamera.h
 * @brief   OrbitCamera class header file
 * @details This file contains the definition of the OrbitCamera class which is responsible for managing an orbiting camera in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-10
 */

#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H

#include "Camera.h"

class OrbitCamera : public Camera
{
public:
    OrbitCamera();

    void onMouseDrag(float deltaX, float deltaY);

    void onMouseScroll(float deltaY);

    void onKeyboard(float deltaTime, bool panLeft, bool panRight, bool panUp, bool panDown);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::mat4 getProjectionMatrix() const;
    /// call this whenever your FBO/window size changes
    void setAspect(float aspect);

private:
    void updateView();

    float _distance;
    glm::vec3 _target;
    float _yaw; // horizontal angle
    float _pitch; // vertical angle
    float _aspect;
};


#endif //ORBITCAMERA_H
