/**
 * @file    Model.cpp
 * @brief   Implementation file for the Model class.
 * @details This file contains the implementation of the Model class which
 *          is responsible for rendering a model. The Model class is a subclass of the Scene class and is used
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#include "Model.h"
#include <rapidjson/document.h>
#include <sstream>
#include <fstream>

// split
//
// Params:  s - string to split
//		    t - string to split (ie. delimiter)
//
//Result:  Splits string according to some substring and returns it as a vector.
std::vector<std::string> split(std::string s, std::string t) {
    std::vector<std::string> res;
    while (true) {
        const int pos = s.find(t);
        if (pos == -1) {
            res.push_back(s);
            break;
        }
        res.push_back(s.substr(0, pos));
        s = s.substr(pos + 1, s.size() - pos - 1);
    }
    return res;
}

Model::Model() : _VAO(0), _VBO(0), _EBO(0) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
}

Model::~Model() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}

bool Model::loadOBJ(const std::string &filename) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;

    if (filename.find(".obj") != std::string::npos) {
        std::ifstream fin(filename, std::ios::in);
        if (!fin) {
            LOG_ERROR("Unable to open file {}", filename);
            return false;
        }

        LOG_INFO("Loading OBJ file {}...", filename);

        std::string lineBuffer;

        while (std::getline(fin, lineBuffer)) {
            std::stringstream ss(lineBuffer);
            std::string cmd;
            ss >> cmd;

            if (cmd == "v") {
                glm::vec3 vertex;
                int dim = 0;
                while (dim < 3 && ss >> vertex[dim]) {
                    dim++;
                }
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

        // close file
        fin.close();

        // For each vertex of each triangle
        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            Vertex ModelVertex;

            // Get the attributes using the indices

            if (tempVertices.size() > 0) {
                glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
                ModelVertex.position = vertex;
            }

            if (tempNormals.size() > 0) {
                glm::vec3 normal = tempNormals[normalIndices[i] - 1];
                ModelVertex.normal = normal;
            }

            if (tempUVs.size() > 0) {
                glm::vec2 uv = tempUVs[uvIndices[i] - 1];
                ModelVertex.texCoords = uv;
            }

            _vertices.push_back(ModelVertex);
        }

        // Create and initailize the buffers
        _setupModel();

        return (_loaded = true);
    }

    return false;
}

void Model::loadJSON(const std::string& jsonString) {
//    rapidjson::Document document;
//    document.Parse(jsonString.c_str());
//
//    if (document.HasMember("vertices") && document["vertices"].IsArray()) {
//        const auto& verts = document["vertices"].GetArray();
//        _vertices.clear();
//        for (const auto& v : verts) {
//            _vertices.emplace_back(v[0].GetFloat(), v[1].GetFloat(), v[2].GetFloat());
//        }
//    }
//
//    if (document.HasMember("texCoords") && document["texCoords"].IsArray()) {
//        const auto& texs = document["texCoords"].GetArray();
//        _texCoords.clear();
//        for (const auto& t : texs) {
//            _texCoords.emplace_back(t[0].GetFloat(), t[1].GetFloat());
//        }
//    }
//
//    if (document.HasMember("normals") && document["normals"].IsArray()) {
//        const auto& norms = document["normals"].GetArray();
//        _normals.clear();
//        for (const auto& n : norms) {
//            _normals.emplace_back(n[0].GetFloat(), n[1].GetFloat(), n[2].GetFloat());
//        }
//    }
//
//    if (document.HasMember("indices") && document["indices"].IsArray()) {
//        const auto& inds = document["indices"].GetArray();
//        _indices.clear();
//        for (const auto& i : inds) {
//            _indices.push_back(i.GetUint());
//        }
//    }
//
//    _setupModel();
}

void Model::bind() const {
    glBindVertexArray(_VAO);
}

void Model::unbind() const {
    glBindVertexArray(0);
}

void Model::draw() const {
    bind();
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    unbind();
}

void Model::_setupModel() {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // unbind to make sure other code does not change it somewhere else
    glBindVertexArray(0);
}
