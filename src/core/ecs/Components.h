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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "../mesh/CubeMesh.h"
#include "../mesh/MeshQuad.h"

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
    explicit TransformComponent(
        const glm::vec3 &pos,
        const glm::vec3 &rot = glm::vec3{0.0f},
        const glm::vec3 &scl = glm::vec3{1.0f}
    )
        : position(pos), rotation(rot), scale(scl) {
    }

    [[nodiscard]] glm::mat4 getMatrix() const {
        auto mat = glm::mat4(1.0f);
        mat = glm::translate(mat, position);
        // Note: GLM rotates in radians, ImGuizmo gives/needs degrees
        mat *= glm::toMat4(glm::quat(glm::radians(rotation)));
        mat = glm::scale(mat, scale);
        return mat;
    }
};

struct QuadComponent {
    MeshQuad mesh;

    QuadComponent() = default;
};

struct CubeComponent {
    CubeMesh mesh;

    CubeComponent() = default;
};

struct TextureComponent {
    Texture texture;
    std::string path;

    TextureComponent() = default;

    explicit TextureComponent(std::string texturePath): path(std::move(texturePath)) {
        texture.loadTexture(path);
    }
};

#endif //COMPONENTS_H
