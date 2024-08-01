/**
 * @file ShaderProgram.cpp
 *
 * @brief Implementation file for the ShaderProgram class.
 *
 * This file contains the implementation of the ShaderProgram class which is responsible for loading and using shader programs.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-08-01
 */

#include "ShaderProgram.h"


ShaderProgram::ShaderProgram() : _programID(0) {}

ShaderProgram::~ShaderProgram() {
    if (_programID != 0) {
        glDeleteProgram(_programID);
    }
}

bool ShaderProgram::loadShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    GLuint vertexShader, fragmentShader;

    // Compile vertex shader
    if (!_compileShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShader)) {
        return false;
    }

    // Compile fragment shader
    if (!_compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader)) {
        return false;
    }

    // Link shaders
    if (!_linkProgram(vertexShader, fragmentShader)) {
        return false;
    }

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void ShaderProgram::use() const {
    if (_programID != 0) {
        glUseProgram(_programID);
    }
}

GLuint ShaderProgram::getProgramID() const {
    return _programID;
}

bool ShaderProgram::_compileShader(const std::string &source, GLenum shaderType, GLuint &shaderID) {
    shaderID = glCreateShader(shaderType);
    const char *shaderSource = source.c_str();
    glShaderSource(shaderID, 1, &shaderSource, nullptr);
    glCompileShader(shaderID);
    _checkCompileErrors(shaderID, "SHADER");
    return true;
}


void ShaderProgram::_checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            LOG_ERROR("Shader compilation error of type: {}", type);
            LOG_ERROR("{}", infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            LOG_ERROR("Program linking error of type: {}", type);
            LOG_ERROR("{}", infoLog);
        }
    }
}

bool ShaderProgram::_linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    _programID = glCreateProgram();
    glAttachShader(_programID, vertexShader);
    glAttachShader(_programID, fragmentShader);
    glLinkProgram(_programID);
    _checkCompileErrors(_programID, "PROGRAM");
    return true;
}





