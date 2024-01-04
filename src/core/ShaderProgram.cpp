#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram()
    : mHandle(0) {
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(mHandle);
}

bool ShaderProgram::LoadShader(const char *vert_shader_filename, const char *frag_shader_filename) {
    std::string vertex_shader_string = FileToString(vert_shader_filename);
    std::string fragment_shader_string = FileToString(frag_shader_filename);

    const GLchar *vertex_shader_ptr = vertex_shader_string.c_str();
    const GLchar *fragment_shader_ptr = fragment_shader_string.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertex_shader_ptr, NULL);
    glShaderSource(fragmentShader, 1, &fragment_shader_ptr, NULL);

    glCompileShader(vertexShader);
    CheckCompileErrors(vertexShader, VERTEX);
    glCompileShader(fragmentShader);
    CheckCompileErrors(fragmentShader, FRAGMENT);

    mHandle = glCreateProgram();
    glAttachShader(mHandle, vertexShader);
    glAttachShader(mHandle, fragmentShader);
    glLinkProgram(mHandle);

    CheckCompileErrors(mHandle, PROGRAM);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    uniformLocations.clear();

    return true;
}

void ShaderProgram::Use() {
    if (mHandle > 0)
        glUseProgram(mHandle);
}

std::string ShaderProgram::FileToString(const std::string &filename) {
    std::stringstream ss;
    std::ifstream file;

    try {
        file.open(filename, std::ios::in);

        if (!file.fail()) {
            ss << file.rdbuf();
        }

        file.close();
    } catch (std::exception ex) {
        std::cerr << "[ERROR] reading shader filename \n";
    }

    return ss.str();
}

void ShaderProgram::CheckCompileErrors(GLuint shader, ShaderType type) {
    int status = 0;

    if (type == PROGRAM) {
        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
            std::string errorlog(length, ' ');
            glGetProgramInfoLog(mHandle, length, &length, &errorlog[0]);
            std::cerr << "[ERROR] program failed to link! " << errorlog << std::endl;
        }
    } else // VERTEX or FRAGMENT
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string errorlog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorlog[0]);
            std::cerr << "[ERROR] shader failed to link! " << errorlog << std::endl;
        }
    }
}

GLuint ShaderProgram::GetProgram() const {
    return mHandle;
}

GLint ShaderProgram::GetUniformLocation(const GLchar *name) {
    std::map<std::string, GLint>::iterator it = uniformLocations.find(name);

    if (it == uniformLocations.end()) {
        uniformLocations[name] = glGetUniformLocation(mHandle, name);
    }

    return uniformLocations[name]; // if we found it, it actually store
}

void ShaderProgram::SetUniform(const GLchar *name, const glm::vec2 &v) {
    GLint loc = GetUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::SetUniform(const GLchar *name, const glm::vec3 &v) {
    GLint loc = GetUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::SetUniform(const GLchar *name, const glm::vec4 &v) {
    GLint loc = GetUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::SetUniform(const GLchar *name, const glm::mat4 &m) {
    GLint loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}
