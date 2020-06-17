#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../vendors/stb-master/stb_image.h"
#include <iostream>

Texture::Texture() : mTexture(0) {}

Texture::~Texture() {}

bool Texture::LoadTexture(const std::string &filename, bool generatingMipMaps)
{
    int width, height, components;

    //stbi_set_flip_vertically_on_load(GL_TRUE); // invert image easy way
    unsigned char *imageData = stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);
    if (imageData == NULL)
    {
        std::cerr << "[ERROR] loading texture" << filename << "' \n";
        return false;
    }

    // invert image // video style
    int widthInBytes = width * 4;
    unsigned char *top = NULL;
    unsigned char *bottom = NULL;
    unsigned char temp = 0;
    int halfHeight = height / 2;
    for (int row = 0; row < halfHeight; row++)
    {
        top = imageData + row * widthInBytes;
        bottom = imageData + (height - row - 1) * widthInBytes;
        for (int col = 0; col < widthInBytes; col++)
        {
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            top++;
            bottom++;
        }
    }

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // t-axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // shrink image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // this streach the image

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); // with mip map they save the minimize image size this save performance

    if (generatingMipMaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);      // free up the memory
    glBindTexture(GL_TEXTURE_2D, 0); // prevent something?
    return true;
}

void Texture::Bind(GLuint texUnit)
{
    glActiveTexture(GL_TEXTURE0 + texUnit); // 16 texture unit in GPU can bind
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::Unbind(GLuint texUnit)
{
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
