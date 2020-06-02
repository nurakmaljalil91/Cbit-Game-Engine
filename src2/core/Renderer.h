#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include "../math/Math.h"

struct DirectionalLight
{
    // Direction of light
    Vector3 mDirection;
    // Diffuse color
    Vector3 mDiffuseColor;
    // Specular color
    Vector3 mSpecColor;
};

class Renderer
{
private:
    SDL_Window *mWindow;    // Window
    SDL_GLContext mContext; // OpenGL context

    float mScreenWidth;
    float mScreenHeight;

    class Game *mGame;

    // Sprite shader
    class Shader *mSpriteShader;
    // Sprite vertex array
    class VertexArray *mSpriteVerts;

    // Mesh shader
    class Shader *mMeshShader;

    // View/projection for 3D shaders
    Matrix4 mView;
    Matrix4 mProjection;

    // Lighting data
    Vector3 mAmbientLight;
    DirectionalLight mDirLight;

    // Map of textures loaded
    std::unordered_map<std::string, class Texture *> mTextures;
    // Map of meshes loaded
    std::unordered_map<std::string, class Mesh *> mMeshes;

    // All the sprite components drawn
    std::vector<class SpriteComponent *> mSprites;

    // All mesh components drawn
    std::vector<class MeshComponent *> mMeshComps;

    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(class Shader *shader);

public:
    Renderer(class Game *game);
    virtual ~Renderer();

    bool Initialize(float screenWidth, float screenHeight);
    void Clear();
    void UnloadData();

    void Draw();

    void AddSprite(class SpriteComponent *sprite);
    void RemoveSprite(class SpriteComponent *sprite);

    void AddMeshComp(class MeshComponent *mesh);
    void RemoveMeshComp(class MeshComponent *mesh);

    class Texture *GetTexture(const std::string &fileName);
    class Mesh *GetMesh(const std::string &fileName);

    void SetViewMatrix(const Matrix4 &view) { mView = view; }

    void SetAmbientLight(const Vector3 &ambient) { mAmbientLight = ambient; }
    DirectionalLight &GetDirectionalLight() { return mDirLight; }

    float GetScreenWidth() const { return mScreenWidth; }
    float GetScreenHeight() const { return mScreenHeight; }
}; // clas Renderer

#endif // RENDERER_H