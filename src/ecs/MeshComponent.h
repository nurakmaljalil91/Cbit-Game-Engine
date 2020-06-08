#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Component.h"
#include "../core/Mesh.h"
#include "../core/Texture.h"
#include <string>

class MeshComponent : public Component
{
private:
    Mesh mMesh;
    Texture mTexture;

public:
    MeshComponent(std::string meshfile, std::string texturefile)
    {
        mMesh.LoadOBJ(meshfile);
        mTexture.LoadTexture(texturefile, true);
    }

    void Initialize() override {}
    void Update(float deltaTime) override {}
    void Render() override
    {

        mTexture.Bind(0);
        mMesh.Draw();
        mTexture.Unbind(0);
    }
};

#endif // COMPONENT_H