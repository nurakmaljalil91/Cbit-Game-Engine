/**
 * @file Camera.h
 * @brief Header file for the Camera class.
 *
 * This file contains the definition of the Camera class which is responsible for managing the camera in the game.
 * The Camera class is responsible for setting up the camera and updating the view matrix.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-08-02
 */

#ifndef CBIT_CAMERA_H
#define CBIT_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void processKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right);

    void processMouseMovement(float xOffset, float yOffset);

    void processMouseScroll(float yOffset);

private:
    void updateCameraVectors();

    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;

    float _yaw;
    float _pitch;
    float _movementSpeed;
    float _mouseSensitivity;
    float _zoom;
};


#endif //CBIT_CAMERA_H
