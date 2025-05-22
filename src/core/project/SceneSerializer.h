/**
 * @file
 * @brief
 * @details
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
    bool loadFromFile(const std::string &filePath);

    void toJson(rapidjson::Document &doc) const;
    void fromJson(const rapidjson::Document &doc) const;
private:
    Scene &_scene;
};


#endif //SCENESERIALIZER_H
