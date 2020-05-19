#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
private:
    unsigned int texture_ID;
    int width;
    int height;

public:
    Texture();
    ~Texture();

    bool Load(const std::string &file_path);
    void Unload();
    void CreateFromSurface(struct SDL_Surface *surface);

    void SetActive();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
}; // class Texture

#endif // TEXTURE_H