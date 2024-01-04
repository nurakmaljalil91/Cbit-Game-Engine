#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
private:
    GLuint mTexture;

    int mWidth;
    int mHeight;

public:
    Texture();

    virtual ~Texture();

    bool LoadTexture(const std::string &filename, bool generatingMipMaps = true);

    void Bind(GLuint texUnit = 0);

    void Unbind(GLuint texUnit = 0);

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
};

#endif //TEXTURE_H
