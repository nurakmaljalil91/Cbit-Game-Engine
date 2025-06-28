/**
 * @file    SceneSerializer.h
 * @brief   SceneSerializer class header fileq
 * @details This file contains the definition of the SceneSerializer class which is responsible for serializing and deserializing scenes to and from JSON format.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-22
 */

#ifndef SCENESERIALIZER_H
#define SCENESERIALIZER_H

#include "Scene.h"
#include <rapidjson/document.h>

class SceneSerializer {
public:
    explicit SceneSerializer(Scene &scene);

    bool saveToFile(const std::string &filePath) const;
    bool loadFromFile(const std::string &filePath) const;

    void toJson(rapidjson::Document &document) const;
    void fromJson(const rapidjson::Document &document) const;
private:
    Scene &_scene;
};


#endif //SCENESERIALIZER_H
