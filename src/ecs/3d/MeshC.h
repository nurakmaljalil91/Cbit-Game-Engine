#ifndef MESHC_H
#define MESHC_H

#include "../Component.h"
#include "../../graphic/Mesh.h"
#include "../../graphic/Texture.h"
#include "../../graphic/VertexArray.h"
#include "../Entity.h"
#include <cstddef>

class MeshC : public Component
{
protected:
    Mesh *mesh;
    size_t texture_index;

public:
    MeshC();
    virtual ~MeshC();

    void Start() override;
    void Handle_Events() override;
    void Update(float delta_time) override;
    void Render(Shader *shader) override;
    void Clear() override;
    // Set the mesh/texture index used by mesh component
    void Set_Mesh(Mesh *_mesh) { mesh = _mesh; }
    void Set_Texture_Index(size_t index) { texture_index = index; }
};

#endif // MESHC_H