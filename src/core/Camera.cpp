/**
 * @file    Camera.cpp
 * @brief   Implementation file for the Camera class.
 * @details This file contains the implementation of the Camera class which is responsible for managing
 *          the camera in the game. The Camera class is responsible for setting up the camera
 *          and updating the view matrix.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-02
 */

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
        _position(position), _worldUp(up), _yaw(yaw),
        _pitch(pitch),
        _front(glm::vec3(0.0f, 0.0f, -1.0f)),
        _movementSpeed(2.5f),
        _mouseSensitivity(0.1), _zoom(45.0f) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(_zoom), aspectRatio, 0.1f, 100.0f);
}

void Camera::processKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right) {
    float velocity = _movementSpeed * deltaTime;
    if (forward) {
        _position += _front * velocity;
    }
    if (backward) {
        _position -= _front * velocity;
    }
    if (left) {
        _position -= _right * velocity;
    }
    if (right) {
        _position += _right * velocity;
    }
}

void Camera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= _mouseSensitivity;
    yOffset *= _mouseSensitivity;

    _yaw += xOffset;
    _pitch += yOffset;

    if (_pitch > 89.0f) {
        _pitch = 89.0f;
    }
    if (_pitch < -89.0f) {
        _pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
    _zoom -= yOffset;
    if (_zoom < 1.0f) {
        _zoom = 1.0f;
    }
    if (_zoom > 45.0f) {
        _zoom = 45.0f;
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}






