/**
 * @file    UICamera.h
 * @brief   UICamera class header file
 * @details UICamera class is responsible for managing the camera used in the user interface (UI).
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-12
 */

#ifndef UICAMERA_H
#define UICAMERA_H

#include "Camera.h"

class UICamera : public Camera {
public:
    UICamera() = default;

    void setOrtho(float left, float right, float bottom, float top, float nearZ = -1.0f, float farZ = 1.0f) {
        _projection = glm::ortho(left, right, bottom, top, nearZ, farZ);
    }

    [[nodiscard]] glm::mat4 getProjectionMatrix() const {
        return _projection;
    }

private:
    glm::mat4 _projection = glm::mat4(1.0f);
};


#endif //UICAMERA_H
