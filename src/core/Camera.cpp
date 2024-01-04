#include "Camera.h"
#include "glm/gtx/transform.hpp"
#include <iostream>

// default camera values
const float DEF_FOV = 45.f;

Camera::Camera()
    : position(glm::vec3(0.f, 0.f, 0.f)),
      targetPosition(glm::vec3(0.f, 0.f, 0.f)),
      up(glm::vec3(0.f, 1.f, 0.f)),
      right(0.f, 0.f, 0.f),
      WORLD_UP(0.f, 1.f, 0.f),
      yaw(glm::pi<float>()),
      pitch(0.f),
      FOV(DEF_FOV)
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, targetPosition, up);
}

const glm::vec3 &Camera::GetLook() const
{
    return look;
}

const glm::vec3 &Camera::GetRight() const
{
    return right;
}

const glm::vec3 &Camera::GetUp() const
{
    return up;
}

FPSCamera::FPSCamera(glm::vec3 fps_cam_pos, float fps_cam_yaw, float fps_cam_pitch)
{
    position = fps_cam_pos;
    yaw = fps_cam_yaw;
    pitch = fps_cam_pitch;
}

void FPSCamera::SetPosition(const glm::vec3 &fps_cam_pos)
{
    position = fps_cam_pos;
}

void FPSCamera::Move(const glm::vec3 &offset_pos)
{
    position += offset_pos;
    UpdateCameraVectors();
}

void FPSCamera::Rotate(float fps_cam_yaw, float fps_cam_pitch)
{
    yaw += glm::radians(fps_cam_yaw);
    pitch += glm::radians(fps_cam_pitch);

    // Constrain the pitch
    pitch = glm::clamp(pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
    UpdateCameraVectors();
} // In degrees

void FPSCamera::UpdateCameraVectors()
{
    // Spherical to Cartesian coordinates
    // https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)

    // Calculate the view direction vector based on yaw and pitch angles (roll not considered)
    // radius is 1 for normalized length
    glm::vec3 cam_look;
    cam_look.x = cosf(pitch) * sinf(yaw);
    cam_look.y = sinf(pitch);
    cam_look.z = cosf(pitch) * cosf(yaw);

    look = glm::normalize(cam_look);

    // Re-calculate the Right and Up vector.  For simplicity the Right vector will
    // be assumed horizontal w.r.t. the world's Up vector.
    right = glm::normalize(glm::cross(look, WORLD_UP));
    up = glm::normalize(glm::cross(right, look));

    targetPosition = position + look;
}

// Orbit Camera
OrbitCamera::OrbitCamera()
    : radius(10.f)
{
}

void OrbitCamera::SetLookAt(const glm::vec3 &target)
{
    targetPosition = target;
}

void OrbitCamera::SetRadius(float _radius)
{
    radius = glm::clamp(_radius, 2.f, 80.f);
}

void OrbitCamera::Rotate(float _yaw, float _pitch)
{
    yaw = glm::radians(_yaw);
    pitch = glm::radians(_pitch);

    pitch = glm::clamp(pitch, -glm::pi<float>() / 2.f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

    UpdateCameraVectors();
    // std::cout << "this is working \n";
}

void OrbitCamera::UpdateCameraVectors()
{
    // std::cout << "this is working \n";
    // Spherical to Cartesian coordinates
    // https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)
    position.x = targetPosition.x + radius * cosf(pitch) * sinf(yaw);
    position.y = targetPosition.y + radius * sinf(pitch);
    position.z = targetPosition.z + radius * cosf(pitch) * cosf(yaw);
}