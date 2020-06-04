#ifndef MESHCOMPONENT2_H
#define MESHCOMPONENT2_H

#include "../Component2.h"

#include "../Shader.h"
#include "../Mesh.h"
#include "../Entity.h"
#include "../../Game.h"
#include "../Renderer.h"
#include "../Texture.h"
#include "../VertexArray.h"
#include <cstddef>
#include <iostream>

class MeshComponent2 : public Component2
{
protected:
    Mesh *mMesh;
    size_t mTextureIndex;

public:
    MeshComponent2(Mesh *mesh)
    {
        this->mMesh = mesh;
        mTextureIndex = 0;
    }

    ~MeshComponent2() {}
    void Start() override {}
    void HandleEvents() override {}
    void Update(float delta_time) override {}

    void Clear() override {}
    // Draw this mesh component
    void Render(Shader *shader) override
    {
        if (mMesh)
        {
            // Set the world transform
            shader->SetMatrixUniform("uWorldTransform",
                                     entity->GetWorldTransform());
            // Set specular power
            shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
            // Set the active texture
            Texture *t = mMesh->GetTexture(mTextureIndex);
            if (t)
            {
                t->SetActive();
            }
            // Set the mesh's vertex array as active
            VertexArray *va = mMesh->GetVertexArray();
            va->SetActive();
            // Draw
            glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
         
        }
    }

    // Set the mesh/texture index used by mesh component
    void SetMesh(Mesh *mesh) { mMesh = mesh; }

    void SetTextureIndex(size_t index) { mTextureIndex = index; }
};

#endif // COMPONENT_H