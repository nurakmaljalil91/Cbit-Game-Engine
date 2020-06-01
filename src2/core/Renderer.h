#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>

class Renderer
{
private:
    SDL_Window *mWindow;    // Window
    SDL_GLContext mContext; // OpenGL context

    float mScreenWidth;
    float mScreenHeight;

    class Game *mGame;

public:
    Renderer(class Game *game);
    virtual ~Renderer();

    bool Initialize(float screenWidth, float screenHeight);
    void Clear();
    void UnloadData();

    void Draw();

    float GetScreenWidth() const { return mScreenWidth; }
    float GetScreenHeight() const { return mScreenHeight; }
}; // clas Renderer

#endif // RENDERER_H