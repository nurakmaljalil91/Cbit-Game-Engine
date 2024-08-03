# Old Source Code for Reference

## Camera class

```c++
// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

class Camera {
protected:
    glm::vec3 position;
    glm::vec3 targetPosition;
    glm::vec3 look;
    glm::vec3 up;
    glm::vec3 right;
    const glm::vec3 WORLD_UP;

    // Eulers angles (in radians)
    float yaw;
    float pitch;

    // Camera parameters
    float FOV; // degree

    Camera();

    virtual void UpdateCameraVectors() {
    }

public:
    glm::mat4 GetViewMatrix() const;

    virtual void SetPosition(const glm::vec3 &cam_pos) {
    }

    virtual void Rotate(float cam_yaw, float cam_pitch) {
    } // in degrees
    virtual void Move(const glm::vec3 &offset_pos) {
    }

    const glm::vec3 &GetLook() const;

    const glm::vec3 &GetRight() const;

    const glm::vec3 &GetUp() const;

    float GetFOV() const { return FOV; }
    void SetFOV(float fov) { FOV = fov; } // In degree
};

class FPSCamera : public Camera {
private:
    void UpdateCameraVectors();

public:
    FPSCamera(glm::vec3 fps_cam_pos = glm::vec3(0.0f, 0.0f, 0.0f), float fps_cam_yaw = glm::pi<float>(),
              float fps_cam_pitch = 0.0f);

    virtual void SetPosition(const glm::vec3 &fps_cam_pos);

    virtual void Rotate(float fps_cam_yaw, float fps_cam_pitch); // In degrees
    virtual void Move(const glm::vec3 &offset_pos);
};

class OrbitCamera : public Camera {
public:
    OrbitCamera();

    virtual void Rotate(float _yaw, float _pitch); // in degrees

    // Camera Controls
    void SetLookAt(const glm::vec3 &target);

    void SetRadius(float _radius);

    glm::vec3 GetPosition() { return position; }

private:
    // Camera parameters
    float radius;

    void UpdateCameraVectors();
};

#endif // CAMERA_H
```

```c++
// Camera.cpp
#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include <iostream>

// default camera values
const float DEF_FOV = 45.f;

Camera::Camera()
    : position(glm::vec3(0.f, 0.f, 0.f)),
      targetPosition(glm::vec3(0.f, 0.f, 0.f)),
      up(glm::vec3(0.f, 1.f, 0.f)),
      right(0.f, 0.f, 0.f),
      WORLD_UP(0.f, 1.f, 0.f),
      yaw(glm::pi<float>()),
      pitch(0.f),
      FOV(DEF_FOV) {
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, targetPosition, up);
}

const glm::vec3 &Camera::GetLook() const {
    return look;
}

const glm::vec3 &Camera::GetRight() const {
    return right;
}

const glm::vec3 &Camera::GetUp() const {
    return up;
}

FPSCamera::FPSCamera(glm::vec3 fps_cam_pos, float fps_cam_yaw, float fps_cam_pitch) {
    position = fps_cam_pos;
    yaw = fps_cam_yaw;
    pitch = fps_cam_pitch;
}

void FPSCamera::SetPosition(const glm::vec3 &fps_cam_pos) {
    position = fps_cam_pos;
}

void FPSCamera::Move(const glm::vec3 &offset_pos) {
    position += offset_pos;
    UpdateCameraVectors();
}

void FPSCamera::Rotate(float fps_cam_yaw, float fps_cam_pitch) {
    yaw += glm::radians(fps_cam_yaw);
    pitch += glm::radians(fps_cam_pitch);

    // Constrain the pitch
    pitch = glm::clamp(pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
    UpdateCameraVectors();
} // In degrees

void FPSCamera::UpdateCameraVectors() {
    // Spherical to Cartesian coordinates
    // https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)

    // Calculate the view direction vector based on yaw and pitch angles (roll not considered)
    // radius is 1 for normalized length
    glm::vec3 cam_look;
    cam_look.x = cosf(pitch) * sinf(yaw);
    cam_look.y = sinf(pitch);
    cam_look.z = cosf(pitch) * cosf(yaw);

    look = glm::normalize(cam_look);

    // Re-calculate the Right and Up vector.  For simplicity the Right vector will
    // be assumed horizontal w.r.t. the world's Up vector.
    right = glm::normalize(glm::cross(look, WORLD_UP));
    up = glm::normalize(glm::cross(right, look));

    targetPosition = position + look;
}

// Orbit Camera
OrbitCamera::OrbitCamera()
    : radius(10.f) {
}

void OrbitCamera::SetLookAt(const glm::vec3 &target) {
    targetPosition = target;
}

void OrbitCamera::SetRadius(float _radius) {
    radius = glm::clamp(_radius, 2.f, 80.f);
}

void OrbitCamera::Rotate(float _yaw, float _pitch) {
    yaw = glm::radians(_yaw);
    pitch = glm::radians(_pitch);

    pitch = glm::clamp(pitch, -glm::pi<float>() / 2.f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

    UpdateCameraVectors();
    // std::cout << "this is working \n";
}

void OrbitCamera::UpdateCameraVectors() {
    // std::cout << "this is working \n";
    // Spherical to Cartesian coordinates
    // https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)
    position.x = targetPosition.x + radius * cosf(pitch) * sinf(yaw);
    position.y = targetPosition.y + radius * sinf(pitch);
    position.z = targetPosition.z + radius * cosf(pitch) * cosf(yaw);
}
```
## Texture Class

```c++
// Texture.h
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
private:
GLuint mTexture;

int mWidth;
int mHeight;

public:
Texture();

virtual ~Texture();

bool LoadTexture(const std::string &filename, bool generatingMipMaps = true);

void Bind(GLuint texUnit = 0);

void Unbind(GLuint texUnit = 0);

int GetWidth() const { return mWidth; }
int GetHeight() const { return mHeight; }
};

#endif //TEXTURE_H
```

```c++
// Texture.cpp

#include
"Texture.h"

#define
STB_IMAGE_IMPLEMENTATION

#include
"stb_image.h"
#include
<iostream>

Texture::Texture() : mTexture(0), mWidth(0), mHeight(0) {}

Texture::~Texture() {
}

bool Texture::LoadTexture(const std::string &filename, bool generatingMipMaps) {
    int components;

    //stbi_set_flip_vertically_on_load(GL_TRUE); // invert image easy way
    unsigned char *imageData = stbi_load(filename.c_str(), &mWidth, &mHeight, &components, STBI_rgb_alpha);
    if (imageData == NULL) {
        std::cerr << "[ERROR] loading texture" << filename << "' \n";
    return false;
}

    // invert image // video style
    int widthInBytes = mWidth * 4;
    unsigned char *top = NULL;
    unsigned char *bottom = NULL;
    unsigned char temp = 0;
    int halfHeight = mHeight / 2;
    for (int row = 0; row < halfHeight; row++) {
    top = imageData + row * widthInBytes;
    bottom = imageData + (mHeight - row - 1) * widthInBytes;
    for (int col = 0; col < widthInBytes; col++) {
    temp = *top;
    *top = *bottom;
    *bottom = temp;
    top++;
    bottom++;
    }
}

glGenTextures(1, &mTexture);
glBindTexture(GL_TEXTURE_2D, mTexture);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t-axis
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // shrink image
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // this streach the image

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
// with mip map they save the minimize image size this save performance

if (generatingMipMaps)
glGenerateMipmap(GL_TEXTURE_2D);

stbi_image_free(imageData); // free up the memory
glBindTexture(GL_TEXTURE_2D, 0); // prevent something?
return true;
}

void Texture::Bind(GLuint texUnit) {
glActiveTexture(GL_TEXTURE0 + texUnit); // 16 texture unit in GPU can bind
glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::Unbind(GLuint texUnit) {
glActiveTexture(GL_TEXTURE0 + texUnit);
glBindTexture(GL_TEXTURE_2D, 0);
}
```
## Mesh Class

```c++
// Mesh.h
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include <glad/glad.h>
#include "glm/glm.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
private:
    bool mLoaded;
    std::vector<Vertex> mVertices;
    GLuint mVBO, mVAO;

    void InitBuffers();

    // ====================== C++ game programming ==================//
    // Textures associated with this mesh
    std::vector<class Texture *> mTextures;
    // Vertex array associated with this mesh
    class VertexArray *mVertexArray;
    // Name of shader specified by mesh
    std::string mShaderName;
    // Stores object space bounding sphere radius
    float mRadius;
    // Specular power of surface
    float mSpecPower;

public:
    Mesh();

    ~Mesh();

    bool LoadOBJ(const std::string &filename);

    void Draw();

    // ====================== C++ game programming ==================//
    // Load/unload mesh
    bool LoadGPH(const std::string &fileName, class Application *renderer);

    void UnloadGPH();

    // Get the vertex array associated with this mesh
    class VertexArray *GetVertexArray() { return mVertexArray; }

    // Get a texture from specified index
    class Texture *GetTexture(size_t index);

    // Get name of shader
    const std::string &GetShaderName() const { return mShaderName; }
    // Get object space bounding sphere radius
    float GetRadius() const { return mRadius; }
    // Get specular power of mesh
    float GetSpecPower() const { return mSpecPower; }
};
#endif //MESH_H
```

```c++
// Mesh.cpp
//-----------------------------------------------------------------------------
// Mesh.cpp by Steve Jones
// Copyright (c) 2015-2016 Game Institute. All Rights Reserved.
//
// Basic Mesh class
//-----------------------------------------------------------------------------
#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

// ================= C++ game programming ============================//
#include "Texture.h"
#include "VertexArray.h"
#include <rapidjson/document.h>
#include <SDL2/SDL_log.h>
#include "../utilities/Math.h"
#include "../Application.h"

//-----------------------------------------------------------------------------
// split
//
// Params:  s - string to split
//		    t - string to split (ie. delimiter)
//
//Result:  Splits string according to some substring and returns it as a vector.
//-----------------------------------------------------------------------------
std::vector<std::string> split(std::string s, std::string t) {
    std::vector<std::string> res;
    while (1) {
        int pos = s.find(t);
        if (pos == -1) {
            res.push_back(s);
            break;
        }
        res.push_back(s.substr(0, pos));
        s = s.substr(pos + 1, s.size() - pos - 1);
    }
    return res;
}

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
Mesh::Mesh()
    : mLoaded(false),
      mVertexArray(nullptr), mRadius(0.0f), mSpecPower(100.0f) {
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
Mesh::~Mesh() {
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

//-----------------------------------------------------------------------------
// Loads a Wavefront OBJ model
//
// NOTE: This is not a complete, full featured OBJ loader.  It is greatly
// simplified.
// Assumptions!
//  - OBJ file must contain only triangles
//  - We ignore materials
//  - We ignore normals
//  - only commands "v", "vt" and "f" are supported
//-----------------------------------------------------------------------------
bool Mesh::LoadOBJ(const std::string &filename) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;

    if (filename.find(".obj") != std::string::npos) {
        std::ifstream fin(filename, std::ios::in);
        if (!fin) {
            std::cerr << "Cannot open " << filename << std::endl;
            return false;
        }

        std::cout << "Loading OBJ file " << filename << " ..." << std::endl;

        std::string lineBuffer;
        while (std::getline(fin, lineBuffer)) {
            std::stringstream ss(lineBuffer);
            std::string cmd;
            ss >> cmd;

            if (cmd == "v") {
                glm::vec3 vertex;
                int dim = 0;
                while (dim < 3 && ss >> vertex[dim])
                    dim++;

                tempVertices.push_back(vertex);
            } else if (cmd == "vt") {
                glm::vec2 uv;
                int dim = 0;
                while (dim < 2 && ss >> uv[dim])
                    dim++;

                tempUVs.push_back(uv);
            } else if (cmd == "vn") {
                glm::vec3 normal;
                int dim = 0;
                while (dim < 3 && ss >> normal[dim])
                    dim++;
                normal = glm::normalize(normal);
                tempNormals.push_back(normal);
            } else if (cmd == "f") {
                std::string faceData;
                int vertexIndex, uvIndex, normalIndex;

                while (ss >> faceData) {
                    std::vector<std::string> data = split(faceData, "/");

                    if (data[0].size() > 0) {
                        sscanf(data[0].c_str(), "%d", &vertexIndex);
                        vertexIndices.push_back(vertexIndex);
                    }

                    if (data.size() >= 1) {
                        // Is face format v//vn?  If data[1] is empty string then
                        // this vertex has no texture coordinate
                        if (data[1].size() > 0) {
                            sscanf(data[1].c_str(), "%d", &uvIndex);
                            uvIndices.push_back(uvIndex);
                        }
                    }

                    if (data.size() >= 2) {
                        // Does this vertex have a normal?
                        if (data[2].size() > 0) {
                            sscanf(data[2].c_str(), "%d", &normalIndex);
                            normalIndices.push_back(normalIndex);
                        }
                    }
                }
            }
        }

        // Close the file
        fin.close();

        // For each vertex of each triangle
        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            Vertex meshVertex;

            // Get the attributes using the indices

            if (tempVertices.size() > 0) {
                glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
                meshVertex.position = vertex;
            }

            if (tempNormals.size() > 0) {
                glm::vec3 normal = tempNormals[normalIndices[i] - 1];
                meshVertex.normal = normal;
            }

            if (tempUVs.size() > 0) {
                glm::vec2 uv = tempUVs[uvIndices[i] - 1];
                meshVertex.texCoords = uv;
            }

            mVertices.push_back(meshVertex);
        }

        // Create and initialize the buffers
        InitBuffers();

        return (mLoaded = true);
    }

    // We shouldn't get here so return failure
    return false;
}

//-----------------------------------------------------------------------------
// Create and initialize the vertex buffer and vertex array object
// Must have valid, non-empty std::vector of Vertex objects.
//-----------------------------------------------------------------------------
void Mesh::InitBuffers() {
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    // Normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // unbind to make sure other code does not change it somewhere else
    glBindVertexArray(0);
}

//-----------------------------------------------------------------------------
// Render the mesh
//-----------------------------------------------------------------------------
void Mesh::Draw() {
    if (!mLoaded)
        return;

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}

// =================== C++ Game Programming ===========================//
bool Mesh::LoadGPH(const std::string &fileName, Application *renderer) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        SDL_Log("File not found: Mesh %s", fileName.c_str());
        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject()) {
        SDL_Log("Mesh %s is not valid json", fileName.c_str());
        return false;
    }

    int ver = doc["version"].GetInt();

    // Check the version
    if (ver != 1) {
        SDL_Log("Mesh %s not version 1", fileName.c_str());
        return false;
    }

    mShaderName = doc["shader"].GetString();

    // Skip the vertex format/shader for now
    // (This is changed in a later chapter's code)
    size_t vertSize = 8;

    // Load textures
    const rapidjson::Value &textures = doc["textures"];
    if (!textures.IsArray() || textures.Size() < 1) {
        SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());
        return false;
    }

    mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

    for (rapidjson::SizeType i = 0; i < textures.Size(); i++) {
        // Is this texture already loaded?
        std::string texName = textures[i].GetString();
//        Texture *t = renderer->getTexture(texName);
//        if (t == nullptr) {
//            // Try loading the texture
//            t = renderer->getTexture(texName);
//            if (t == nullptr) {
//                // If it's still null, just use the default texture
//                t = renderer->getTexture("Assets/Default.png");
//            }
//        }
//        mTextures.emplace_back(t);
    }

    // Load in the vertices
    const rapidjson::Value &vertsJson = doc["vertices"];
    if (!vertsJson.IsArray() || vertsJson.Size() < 1) {
        SDL_Log("Mesh %s has no vertices", fileName.c_str());
        return false;
    }

    std::vector<float> vertices;
    vertices.reserve(vertsJson.Size() * vertSize);
    mRadius = 0.0f;
    for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
        // For now, just assume we have 8 elements
        const rapidjson::Value &vert = vertsJson[i];
        if (!vert.IsArray() || vert.Size() != 8) {
            SDL_Log("Unexpected vertex format for %s", fileName.c_str());
            return false;
        }

        Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
        mRadius = Math::Max(mRadius, pos.LengthSq());

        // Add the floats
        for (rapidjson::SizeType i = 0; i < vert.Size(); i++) {
            vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
        }
    }

    // We were computing length squared earlier
    mRadius = Math::Sqrt(mRadius);

    // Load in the indices
    const rapidjson::Value &indJson = doc["indices"];
    if (!indJson.IsArray() || indJson.Size() < 1) {
        SDL_Log("Mesh %s has no indices", fileName.c_str());
        return false;
    }

    std::vector<unsigned int> indices;
    indices.reserve(indJson.Size() * 3);
    for (rapidjson::SizeType i = 0; i < indJson.Size(); i++) {
        const rapidjson::Value &ind = indJson[i];
        if (!ind.IsArray() || ind.Size() != 3) {
            SDL_Log("Invalid indices for %s", fileName.c_str());
            return false;
        }

        indices.emplace_back(ind[0].GetUint());
        indices.emplace_back(ind[1].GetUint());
        indices.emplace_back(ind[2].GetUint());
    }

    // Now create a vertex array
    mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
                                   indices.data(), static_cast<unsigned>(indices.size()));
    return true;
}

void Mesh::UnloadGPH() {
    delete mVertexArray;
    mVertexArray = nullptr;
}

Texture *Mesh::GetTexture(size_t index) {
    if (index < mTextures.size()) {
        return mTextures[index];
    } else {
        return nullptr;
    }
}
```
## VertexArray Class

```c++  
// VertexArray.h
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

class VertexArray {
public:
    VertexArray(const float *verts, unsigned int numVerts,
                const unsigned int *indices, unsigned int numIndices);

    ~VertexArray();

    void SetActive();

    unsigned int GetNumIndices() const { return mNumIndices; }
    unsigned int GetNumVerts() const { return mNumVerts; }

private:
    // How many vertices in the vertex buffer?
    unsigned int mNumVerts;
    // How many indices in the index buffer
    unsigned int mNumIndices;
    // OpenGL ID of the vertex buffer
    unsigned int mVertexBuffer;
    // OpenGL ID of the index buffer
    unsigned int mIndexBuffer;
    // OpenGL ID of the vertex array object
    unsigned int mVertexArray;
};

#endif // VERTEXARRAY_H
```

```c++

// VertexArray.cpp
#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray(const float *verts, unsigned int numVerts,
                         const unsigned int *indices, unsigned int numIndices)
    : mNumVerts(numVerts), mNumIndices(numIndices) {
    // Create vertex array
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    // Create vertex buffer
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVerts * 8 * sizeof(float), verts, GL_STATIC_DRAW);

    // Create index buffer
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Specify the vertex attributes
    // (For now, assume one vertex format)
    // Position is 3 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    // Normal is 3 floats
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          reinterpret_cast<void *>(sizeof(float) * 3));
    // Texture coordinates is 2 floats
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          reinterpret_cast<void *>(sizeof(float) * 6));
}

VertexArray::~VertexArray() {
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive() {
    glBindVertexArray(mVertexArray);
}
```
