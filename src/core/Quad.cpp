/**
 * @file    Quad.cpp
 * @brief   Implementation of the Quad class.
 * @details Implementation of the Quad class which is used to represent a 2D quad.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-23
 */

#include "Quad.h"

Quad::Quad(): _x(0.0f),
              _y(0.0f),
              _width(1.0f),
              _height(1.0f),
              _initialized(false) {
}

Quad::Quad(const float x, const float y, const float width, const float height): _x(x),
    _y(y),
    _width(width),
    _height(height),
    _initialized(false) {
    _vao.initialize();
    set(x, y, width, height);
}

Quad::~Quad() = default;


void Quad::set(const float x, const float y, const float width, const float height) {
    if (!_initialized) {
        _vao.initialize();
        _initialized = true;
    }
    _x = x; _y = y; _width = width; _height = height;
    _updateBuffer();
}

void Quad::_updateBuffer() {
    // Calculate corner positions
    const float x1 = _x;
    const float y1 = _y;
    const float x2 = _x + _width;
    const float y2 = _y + _height;

    // Interleaved position (X, Y) and UV (U, V):
    GLfloat vertices[] = {
        x1, y1, 0.0f, 1.0f,   // bottom-left
        x1, y2, 0.0f, 0.0f,   // top-left
        x2, y1, 1.0f, 1.0f,   // bottom-right
        x2, y2, 1.0f, 0.0f    // top-right
    };

    // Upload to GPU: 2 floats for pos, 2 for UV :contentReference[oaicite:3]{index=3}
    _vao.addBuffer(vertices, sizeof(vertices), std::vector<GLuint>{2, 2});
}

void Quad::draw() const {
    _vao.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    _vao.unbind();
}
