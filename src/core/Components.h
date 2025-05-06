/**
 * @file    Components.h
 * @brief   Header file for the Components struct.
 * @details This file contains the definition of the Components struct which is responsible
 *          for storing the components of an entity.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-06
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <glm/glm.hpp>

#include "CubeMesh.h"
#include "MeshQuad.h"

struct TagComponent {
    std::string tag;
};

struct IdComponent {
    std::string uuid;
};

struct TransformComponent {
    glm::vec3 position{0.0f, 0.0f, 0.0f}; // world space
    glm::vec3 rotation{0.0f, 0.0f, 0.0f}; // Euler angles in degrees
    glm::vec3 scale{1.0f, 1.0f, 1.0f}; // non‐uniform scale

    // Default ctor: identity transform
    TransformComponent() = default;

    // Convenient ctor: set pos, rot, scale in one go
    TransformComponent(
        const glm::vec3 &pos,
        const glm::vec3 &rot = glm::vec3{0.0f},
        const glm::vec3 &scl = glm::vec3{1.0f}
    )
        : position(pos), rotation(rot), scale(scl) {
    }
};

struct QuadComponent {
    MeshQuad mesh;

    QuadComponent() = default;

    // QuadComponent(
    //     const Vector3 position = Vector3(0.0f, 0.0f, 0.0f),
    //     const Vector2 size = Vector2(1.0f, 1.0f),
    //     const float rotation = 0.0f,
    //     const Vector3 rotationAxis = Vector3(0.0f, 0.0f, 0.0f),
    //     const Vector3 scale = Vector3(1.0f, 1.0f, 1.0f),
    //     const Vector3 color = Vector3(1.0f, 1.0f, 1.0f)
    // ) {
    //     mesh.setPosition({position.x, position.y, position.z});
    //     mesh.setSize({size.x, size.y});
    //     mesh.setRotation(rotation, {rotationAxis.x, rotationAxis.y, rotationAxis.z});
    //     mesh.setScale({scale.x, scale.y, scale.z});
    //     mesh.setColor({color.x, color.y, color.z, 1.0f});
    // }
    //
    // void setPosition(const Vector3 &pos) {
    //     mesh.setPosition({pos.x, pos.y, pos.z});
    // }
};

struct CubeComponent {
    CubeMesh mesh;

    CubeComponent() = default;
};

#endif //COMPONENTS_H
