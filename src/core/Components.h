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
#include "Quad.h"

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
    TransformComponent(const glm::vec3 &pos,
                       const glm::vec3 &rot = glm::vec3{0.0f},
                       const glm::vec3 &scl = glm::vec3{1.0f})
        : position(pos), rotation(rot), scale(scl) {
    }
};

struct QuadComponent {
    Quad mesh;
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    // default: unit quad at (0,0)
    QuadComponent() = default;

    QuadComponent(const glm::vec4 &c) : color(c) {
    }
};

#endif //COMPONENTS_H
