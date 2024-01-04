#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>
#include <string>
#include "glm/glm.hpp"
#include <map>

enum ShaderType {
    VERTEX,
    FRAGMENT,
    PROGRAM
};

class ShaderProgram {
private:
    GLuint mHandle;

    std::map<std::string, GLint> uniformLocations;

    std::string FileToString(const std::string &filename);

    void CheckCompileErrors(GLuint shader, ShaderType type);

    GLint GetUniformLocation(const GLchar *name);

public:
    ShaderProgram();

    ~ShaderProgram();

    bool LoadShader(const char *vert_shader_filename, const char *frag_shader_filename);

    void Use();

    void SetUniform(const GLchar *name, const glm::vec2 &v);

    void SetUniform(const GLchar *name, const glm::vec3 &v);

    void SetUniform(const GLchar *name, const glm::vec4 &v);

    void SetUniform(const GLchar *name, const glm::mat4 &m);

    GLuint GetProgram() const;
}; // class ShaderProgram

#endif // SHADERPROGRAM_H
