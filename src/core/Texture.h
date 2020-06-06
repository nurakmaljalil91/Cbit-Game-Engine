#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture
{
public:
    Texture();
    virtual ~Texture();

    bool LoadTexture(const std::string &filename, bool generatingMipMaps = true);
    void Bind(GLuint texUnit = 0);
    void Unbind(GLuint texUnit = 0);

private:
    GLuint mTexture;
};

#endif //TEXTURE_H