//
// Created by User on 20/4/2025.
//

#include "TextRenderer.h"
#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>

#include "SDL2/SDL_ttf.h"

TextRenderer::TextRenderer(unsigned int screenWidth,
                           unsigned int screenHeight) {
    // 1) Compile & configure shader
    _textShader.loadShader("resources/shaders/text.vert",
                           "resources/shaders/text.frag");
    _textShader.use();
    _projection = glm::ortho(0.0f, (float) screenWidth,
                             0.0f, (float) screenHeight);
    // set it once
    glUniformMatrix4fv(
        glGetUniformLocation(_textShader.getProgramID(), "projection"),
        1, GL_FALSE, &_projection[0][0]
    );
    glUniform1i(
        glGetUniformLocation(_textShader.getProgramID(), "text"),
        0
    );

    // 2) Configure _vao/_vbo for texture quads
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // 6 vertices × 4 floats (x,y, u,v)
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer() {
    // Delete glyph textures
    for (auto &pair: _characters) {
        glDeleteTextures(1, &pair.second.TextureID);
    }
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

bool TextRenderer::loadFont(const std::string &fontPath,
                            unsigned int fontSize) {
    TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) return false;

    // Load first 128 ASCII _characters
    for (unsigned char c = 0; c < 128; ++c) {
        // render glyph to SDL_Surface
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface *surf = TTF_RenderGlyph_Blended(font, c, white);
        if (!surf) continue;

        // convert to RGBA8888
        SDL_Surface *conv = SDL_ConvertSurfaceFormat(
            surf, SDL_PIXELFORMAT_RGBA8888, 0
        );
        SDL_FreeSurface(surf);

        // generate OpenGL texture
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            conv->w,
            conv->h,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            conv->pixels
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



        // get metrics
        int minx, maxx, miny, maxy, advance;
        TTF_GlyphMetrics(font, c,
                         &minx, &maxx,
                         &miny, &maxy,
                         &advance);

        // store character
        Character chr = {
            tex,
            {conv->w, conv->h},
            {minx, maxy},
            static_cast<GLuint>(advance)
        };
        _characters.insert({(char) c, chr});
        SDL_FreeSurface(conv);
    }

    TTF_CloseFont(font);
    return true;
}

void TextRenderer::renderText(const std::string &text,
                              float x, float y,
                              float scale,
                              const glm::vec3 &color) {
    _textShader.use();
    glUniform3f(
        glGetUniformLocation(_textShader.getProgramID(), "textColor"),
        color.x, color.y, color.z
    );
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);

    // iterate through _characters
    for (auto c: text) {
        Character &ch = _characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // update _vbo for each character
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of _vbo memory
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        sizeof(vertices),
                        vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // draw quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // advance cursors for next glyph
        x += ch.Advance * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
