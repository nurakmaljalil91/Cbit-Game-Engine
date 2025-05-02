/**
 * @file    ShaderManager.h
 * @brief   ShaderManager class header file.
 * @details ShaderManager class is responsible for managing shader programs in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <string>
#include <unordered_map>

#include "ShaderProgram.h"

class ShaderManager {
public:
    bool loadFromFile(const std::string &name,
                      const std::string &vertexShaderPath,
                      const std::string &fragmentShaderPath
    );

    bool loadFromSource(const std::string &name,
                        const std::string &vertexShaderSource,
                        const std::string &fragmentShaderSource
    );

    std::shared_ptr<ShaderProgram> get(const std::string &name);

private:
    std::unordered_map<std::string, std::shared_ptr<ShaderProgram> > _shaders;
};


#endif //SHADERMANAGER_H
