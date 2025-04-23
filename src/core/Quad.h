/**
 * @file    Quad.h
 * @brief   Header file for the Quad class.
 * @details This file contains the definition of the Quad class which is used to represent a 2D quad.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-23
 */

#ifndef QUAD_H
#define QUAD_H

#include "VertexArray.h"

class Quad {
public:
    Quad();

    Quad(float x, float y, float width, float height);

    ~Quad();

    void set(float x, float y, float width, float height);

    void draw() const;

private:
    VertexArray _vao;
    float _x, _y, _width, _height;
    bool _initialized;

    void _updateBuffer();
};


#endif //QUAD_H
