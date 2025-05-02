/**
 * @file    ShaderManager.cpp
 * @brief   ShaderManager class implementation file.
 * @details ShaderManager class is responsible for managing shader programs in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */

#include "ShaderManager.h"

bool ShaderManager::loadFromFile(const std::string &name, const std::string &vertexShaderPath,
                                 const std::string &fragmentShaderPath) {
    const auto shader = std::make_shared<ShaderProgram>();
    if (!shader->loadShader(vertexShaderPath, fragmentShaderPath))
        return false;
    _shaders[name] = shader;
    return true;
}

bool ShaderManager::loadFromSource(const std::string &name, const std::string &vertexShaderSource,
                                   const std::string &fragmentShaderSource) {
    const auto shader = std::make_shared<ShaderProgram>();
    if (!shader->loadFromSource(vertexShaderSource, fragmentShaderSource))
        return false;
    _shaders[name] = shader;
    return true;
}

std::shared_ptr<ShaderProgram> ShaderManager::get(const std::string &name) {
    return _shaders.at(name);
}
