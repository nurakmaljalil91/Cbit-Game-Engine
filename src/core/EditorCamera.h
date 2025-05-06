/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-06
 */

#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include "Camera.h"

class EditorCamera : public Camera {
public:
    EditorCamera();

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


#endif //EDITORCAMERA_H
