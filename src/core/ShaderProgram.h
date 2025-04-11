/**
 * @file    ShaderProgram.h
 * @brief   Header file for the ShaderProgram class.
 * @details This file contains the definition of the ShaderProgram class which is responsible for loading and using shader programs.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-01
 */

#ifndef CBIT_SHADERPROGRAM_H
#define CBIT_SHADERPROGRAM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "../utilities/Logger.h"

class ShaderProgram {
public:
    ShaderProgram();

    ~ShaderProgram();

    bool loadShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

    void use() const;

    [[nodiscard]] GLuint getProgramID() const;

private:
    GLuint _programID;

    static bool _compileShader(const std::string &source, GLenum shaderType, GLuint &shaderID);

    bool _linkProgram(GLuint vertexShader, GLuint fragmentShader);

    static void _checkCompileErrors(GLuint shader, std::string type);

    static std::string _readFile(const std::string &filePath);
}; // class ShaderProgram

#endif // CBIT_SHADERPROGRAM_H
