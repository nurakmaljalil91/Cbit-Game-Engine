/**
 * @file    Lighting.h
 * @brief   Header file for the Lighting class
 * @details This file contains the definition of the Lighting class which is responsible for setting up lighting in the scene.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-18
 */

#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>
#include "ShaderProgram.h"

class Lighting {
public:
    static void applyBasicDirectionalLight(ShaderProgram &shader, const glm::vec3 &viewPos) {
        shader.use();
        shader.setVec3("lightDir", glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
        shader.setVec3("lightColor", glm::vec3(1.0f));
        shader.setVec3("ambientColor", glm::vec3(0.2f));
        shader.setFloat("shininess", 32.0f);
        shader.setVec3("viewPos", viewPos);
    }

    // Extend this later with point lights, spotlights, etc.
};

#endif //LIGHTING_H
