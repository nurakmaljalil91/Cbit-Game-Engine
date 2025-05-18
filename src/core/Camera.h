/**
 * @file    Camera.h
 * @brief   Header file for the Camera class.
 * @details This file contains the definition of the Camera class which is responsible for managing
 *          the camera in the game. The Camera class is responsible for setting up the camera and updating
 *          the view matrix.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-02
 */

#ifndef CBIT_CAMERA_H
#define CBIT_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera() = default; // now trivial
    ~Camera() = default;
    
    // call this once after default construction
    void setup(glm::vec3 position,
               glm::vec3 worldUp,
               float yawDegrees,
               float pitchDegrees,
               float movementSpeed = 2.5f,
               float mouseSensitivity = 0.1f,
               float zoomDegrees = 45.0f);

    // NOTE: creating a constructor to setup.
    Camera( const glm::vec3&,
            const glm::vec3&,
            const float,
            const float,
            const float = 2.5,
            const float = 0.1f,
            const float = 45.f
            );

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void processKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right);

    void processMouseMovement(float xOffset, float yOffset);

    void processMouseScroll(float yOffset);

    glm::vec3 getPosition() const { return _position; }

protected:
    void updateCameraVectors();

    // stored state
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 _right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float _yaw = -90.0f; // default look toward -Z
    float _pitch = 0.0f;
    float _movementSpeed = 2.5f;
    float _mouseSensitivity = 0.1f;
    float _zoom = 45.0f;
};


#endif //CBIT_CAMERA_H
