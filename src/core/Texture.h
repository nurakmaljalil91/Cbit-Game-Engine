/**
 * @file    Texture.h
 * @brief   Header file for the Texture class.
 * @details This file contains the definition of the Texture class, which is used to represent
 *          a texture in the game. The Texture class is responsible for loading and binding textures
 *          to the rendering pipeline.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#ifndef CBIT_TEXTURE_H
#define CBIT_TEXTURE_H

#include <glad/glad.h>
#include <string>

#include "../utilities/Logger.h"


class Texture {
public:
    Texture();

    ~Texture();

    bool loadTexture(const std::string &path);

    void bind() const;

private:
    GLuint _textureID;
};


#endif //CBIT_TEXTURE_H
