/**
 * @file    CubeMesh.h
 * @brief   Header file for the CubeMesh class
 * @details This file contains the definition of the CubeMesh class which is used to represent a 3D cube mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-06
 */
#ifndef CUBEMESH_H
#define CUBEMESH_H
#include "Mesh.h"


class CubeMesh : public Mesh {
public:
    CubeMesh();

    ~CubeMesh() override;

protected:
    void setupMesh() override;
    bool _initialized = false;
};


#endif //CUBEMESH_H
