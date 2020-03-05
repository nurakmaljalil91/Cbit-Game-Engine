#include "MeshC.h"

MeshC::MeshC() : mesh(nullptr), texture_index(0) {}
MeshC::~MeshC() {}

void MeshC::Start() {}
void MeshC::Handle_Events() {}
void MeshC::Update(float delta_time) {}
void MeshC::Render(Shader *shader)
{
    if (mesh)
    {
        shader->Set_Matrix_Uniform("world_transform", entity->Get_World_Transform());
        shader->Set_Float_Uniform("specular_power", mesh->Get_Specular_Power);
        Texture *texture = mesh->Get_Texture(texture_index);
        if (texture)
        {
            texture->SetActive();
        }
        VertexArray *vertex_array = mesh->Get_Vertex_Array();
        vertex_array->SetActive();
        glDrawElements(GL_TRIANGLES, vertex_array->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}
void MeshC::Clear() {}