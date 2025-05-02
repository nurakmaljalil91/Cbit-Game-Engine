/**
 * @file   Component.h
 * @brief  Header file for the Component struct.
 * @detail This file contains the definition of the Components struct which is responsible for storing
 *         the components of an entity.The Components struct is responsible for storing the components of an entity.
 * @author Nur Akmal bin Jalil
 * @date 2024-07-25
 */

#ifndef COMPONENT_H
#define COMPONENT_H

#include "../core/ShaderProgram.h"

class Entity;

class Component final
{
public:
    Entity *owner{};

    virtual ~Component() = default;

    virtual void Initialize() {
    }

    virtual void HandleEvents() {
    }

    virtual void Update(float deltaTime) {
    }

    virtual void Render(ShaderProgram *shader) {
    }
};

#endif
