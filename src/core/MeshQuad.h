/**
 * @file    MeshQuad.h
 * @brief   Header file for the MeshQuad class
 * @details This file contains the definition of the MeshQuad class which is used to represent a 2D quad mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */


#ifndef MESHQUAD_H
#define MESHQUAD_H

#include "Mesh.h"

class MeshQuad : public Mesh {
public:
    MeshQuad();

    ~MeshQuad() override;

protected:
    void setupMesh() override;
};


#endif //MESHQUAD_H
