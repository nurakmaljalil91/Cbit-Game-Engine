#include "MeshComponent.h"
#include "../core/Shader.h"
#include "../core/Mesh.h"
#include "Actor.h"
#include "../Game.h"
#include "../core/Renderer.h"
#include "../core/Texture.h"
#include "../core/VertexArray.h"

MeshComponent::MeshComponent(Actor *owner)
    : Component(owner), mMesh(nullptr), mTextureIndex(0)
{
    mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
    mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader *shader)
{
    if (mMesh)
    {
        // Set the world transform
        shader->SetMatrixUniform("uWorldTransform",
                                 mOwner->GetWorldTransform());
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