/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-10
 */

#include "OrbitCamera.h"

#include <glm/gtc/matrix_transform.hpp>

OrbitCamera::OrbitCamera(): _distance(10.0f),
                            _target(0.0f, 0.0f, 0.0f),
                            _yaw(0.0f),
                            _pitch(20.0f),
                            _aspect(1.0f)
{
    // initialize base Camera with fake pos/up; updateView() will override _position
    setup(
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        _yaw,
        _pitch
        // movementSpeed, mouseSensitivity, zoomDegrees use their defaults
    );
    updateView();
}

void OrbitCamera::onMouseDrag(float dx, float dy) {
    // rotate around target
    _yaw += dx * 0.25f;
    _pitch += dy * 0.25f;
    _pitch = glm::clamp(_pitch, -89.0f, 89.0f);
    updateView();
}

void OrbitCamera::onMouseScroll(float dy) {
    _distance = glm::max(1.0f, _distance - dy * 0.5f);
    updateView();
}

void OrbitCamera::onKeyboard(float dt, bool left, bool right, bool up, bool down) {
    // pan target
    float panSpeed = _distance * dt;
    // camera's right and up vectors
    glm::vec3 camRight = _right;
    glm::vec3 camUp = _up;
    if (left) _target -= camRight * panSpeed;
    if (right) _target += camRight * panSpeed;
    if (up) _target += camUp * panSpeed;
    if (down) _target -= camUp * panSpeed;
    updateView();
}

glm::mat4 OrbitCamera::getViewMatrix() const {
    return Camera::getViewMatrix();
}

glm::mat4 OrbitCamera::getProjectionMatrix() const {
    // use the same near/far as your main perspective
    return glm::perspective(glm::radians(_zoom), _aspect, 0.1f, 1000.0f);
}

void OrbitCamera::setAspect(float aspect) {
    _aspect = aspect;
}

void OrbitCamera::updateView() {
    // convert spherical coords to Cartesian
    float yawR = glm::radians(_yaw);
    float pitchR = glm::radians(_pitch);

    glm::vec3 pos;
    pos.x = _target.x + _distance * cos(pitchR) * cos(yawR);
    pos.y = _target.y + _distance * sin(pitchR);
    pos.z = _target.z + _distance * cos(pitchR) * sin(yawR);

    // update base Camera
    _position = pos;
    _front = glm::normalize(_target - _position);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}
