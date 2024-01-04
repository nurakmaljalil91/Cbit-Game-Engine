#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

class Camera {
protected:
    glm::vec3 position;
    glm::vec3 targetPosition;
    glm::vec3 look;
    glm::vec3 up;
    glm::vec3 right;
    const glm::vec3 WORLD_UP;

    // Eulers angles (in radians)
    float yaw;
    float pitch;

    // Camera parameters
    float FOV; // degree

    Camera();

    virtual void UpdateCameraVectors() {
    }

public:
    glm::mat4 GetViewMatrix() const;

    virtual void SetPosition(const glm::vec3 &cam_pos) {
    }

    virtual void Rotate(float cam_yaw, float cam_pitch) {
    } // in degrees
    virtual void Move(const glm::vec3 &offset_pos) {
    }

    const glm::vec3 &GetLook() const;

    const glm::vec3 &GetRight() const;

    const glm::vec3 &GetUp() const;

    float GetFOV() const { return FOV; }
    void SetFOV(float fov) { FOV = fov; } // In degree
};

class FPSCamera : public Camera {
private:
    void UpdateCameraVectors();

public:
    FPSCamera(glm::vec3 fps_cam_pos = glm::vec3(0.0f, 0.0f, 0.0f), float fps_cam_yaw = glm::pi<float>(),
              float fps_cam_pitch = 0.0f);

    virtual void SetPosition(const glm::vec3 &fps_cam_pos);

    virtual void Rotate(float fps_cam_yaw, float fps_cam_pitch); // In degrees
    virtual void Move(const glm::vec3 &offset_pos);
};

class OrbitCamera : public Camera {
public:
    OrbitCamera();

    virtual void Rotate(float _yaw, float _pitch); // in degrees

    // Camera Controls
    void SetLookAt(const glm::vec3 &target);

    void SetRadius(float _radius);

    glm::vec3 GetPosition() { return position; }

private:
    // Camera parameters
    float radius;

    void UpdateCameraVectors();
};

#endif // CAMERA_H
