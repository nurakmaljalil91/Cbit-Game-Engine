#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
    Texture();
    ~Texture();

    bool Load(const std::string &fileName);
    void Unload();

    void SetActive();

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }

private:
    unsigned int mTextureID;
    int mWidth;
    int mHeight;
};

#endif // TEXTURE_H