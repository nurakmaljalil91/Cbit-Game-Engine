/**
 * @file    TextRenderer.h
 * @brief   Class for rendering text using FreeType and OpenGL.
 * @details This file contains the definition of the TextRenderer class which is responsible for rendering
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-20
 */

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <map>
#include <string>
#include "ShaderProgram.h"
#include "glad/glad.h"
#include "glm/vec2.hpp"


struct Character {
    GLuint TextureID; // ID handle of the glyph texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance; // Horizontal offset to advance to next glyph
};

class TextRenderer {
public:
    // screenWidth/screenHeight in pixels for the projection matrix
    TextRenderer(unsigned int screenWidth, unsigned int screenHeight);

    ~TextRenderer();

    // Load a font at given pixel-size. Returns false on failure.
    bool loadFont(const std::string &fontPath, unsigned int fontSize);

    // Render text at (x,y) in pixel coords, scaled by 'scale', tinted by 'color' (0..1 floats).
    void renderText(const std::string &text,
                    float x, float y,
                    float scale,
                    const glm::vec3 &color);

private:
    std::map<GLchar, Character> _characters;
    GLuint _vao, _vbo;
    ShaderProgram _textShader;
    glm::mat4 _projection;
};


#endif //TEXTRENDERER_H
