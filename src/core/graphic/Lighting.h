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

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    glm::vec3 ambient;
};

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(17.5f));
};

class Lighting {
public:
    static void applyDirectionalLight(ShaderProgram &shader, const DirectionalLight &light, const glm::vec3 &viewPos) {
        shader.setVec3("lightDir", glm::normalize(light.direction));
        shader.setVec3("lightColor", light.color);
        shader.setVec3("ambientColor", light.ambient);
        shader.setFloat("shininess", 32.0f);
        shader.setVec3("viewPos", viewPos);
    }

    static void applyPointLight(ShaderProgram &shader, const PointLight &light, const glm::vec3 &viewPos) {
        shader.setVec3("pointLight.position", light.position);
        shader.setVec3("pointLight.color", light.color);
        shader.setFloat("pointLight.constant", light.constant);
        shader.setFloat("pointLight.linear", light.linear);
        shader.setFloat("pointLight.quadratic", light.quadratic);
        shader.setVec3("viewPos", viewPos);
    }

    static void applySpotLight(ShaderProgram &shader, const SpotLight &light, const glm::vec3 &viewPos) {
        shader.setVec3("spotLight.position", light.position);
        shader.setVec3("spotLight.direction", light.direction);
        shader.setVec3("spotLight.color", light.color);
        shader.setFloat("spotLight.cutOff", light.cutOff);
        shader.setFloat("spotLight.outerCutOff", light.outerCutOff);
        shader.setVec3("viewPos", viewPos);
    }
};

#endif //LIGHTING_H
