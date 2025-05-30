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
#include <string>
#include <glm/glm.hpp>
#include "../../utilities/Logger.h"

class ShaderProgram {
public:
    ShaderProgram();

    ~ShaderProgram();

    bool loadShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

    /**
     * @brief   Loads and compiles shader programs from source code.
     * @details This function takes the source code for a vertex shader and a fragment shader,
     *          compiles them, and links them into a shader program.
     * 
     * @param   vertexShaderSource The source code for the vertex shader.
     * @param   fragmentShaderSource The source code for the fragment shader.
     * @return  True if the shaders were successfully compiled and linked; false otherwise.
     * @note    Logs errors using the Logger utility if compilation or linking fails.
     */
    bool loadFromSource(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

    void use() const;

    [[nodiscard]] GLuint getProgramID() const;

    void setMat4(const std::string &uniformName, const glm::mat4 &matrix) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;

    void setFloat(const std::string &name, float value) const;

    void setInt(const std::string &name, int value) const;

    void setBool(const std::string &name, bool value) const;

private:
    GLuint _programID;
    mutable std::unordered_map<std::string, GLint> _uniformCache;

    GLint _getUniformLocation(const std::string &name) const;

    static bool _compileShader(const std::string &source, GLenum shaderType, GLuint &shaderID);

    bool _linkProgram(GLuint vertexShader, GLuint fragmentShader);

    static void _checkCompileErrors(GLuint shader, std::string type);

    static std::string _readFile(const std::string &filePath);
}; // class ShaderProgram

#endif // CBIT_SHADERPROGRAM_H
