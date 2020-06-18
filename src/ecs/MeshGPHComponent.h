#ifndef MESHGPHCOMPONENT_H
#define MESHGPHCOMPONENT_H

#include "Component.h"
#include "../core/Mesh.h"
#include "../core/Texture.h"
#include "../core/VertexArray.h"

#include <string>

class MeshGPHComponent : public Component
{
private:
    Mesh *mMesh;
    size_t mTextureIndex;

public:
    MeshGPHComponent(Mesh *mesh, size_t index)
    {
        mMesh = mesh;
        mTextureIndex = index;
    }
    ~MeshGPHComponent() {}

    void Initialize() override {}
    void Update(float deltaTime) override {}
    void Render(ShaderProgram *shader) override
    {
        if (mMesh)
        {
            shader->SetUniform("model", owner->GetWorldPosition());
            Texture *t = mMesh->GetTexture(mTextureIndex);
            if (t)
            {
                t->Bind();
            }
            VertexArray *va = mMesh->GetVertexArray();
            va->SetActive();

            // Draw
            glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
        }
    }
};

#endif // MESHGPHCOMPONENT_H