#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Renderer.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Load(const &std::string &filename, Renderer *renderer); // Load Mesh
    void Unload();                                               // Unload the mesh
};

#endif // MESH_H