#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include "../math/Math.h"

class Shader
{
public:
    Shader();
    ~Shader();
    // Load the vertex/fragment shaders with the given names
    bool Load(const std::string &_vertex_name, const std::string &_fragmet_name);
    void Unload();

    // Set this as the active shader program
    void Set_Active();

    // Sets a Matrix uniform
    void Set_Matrix_Uniform(const char *_name, const Matrix4 &_matrix);

    // Sets a Vector3 uniform
    void Set_Vector_Uniform(const char *_name, const Vector3 &vector);

    // Sets a float uniform
    void Set_Float_Uniform(const char *_name, float value);

private:
    // Tries to compile the specified shader
    bool CompileShader(const std::string &file_name, GLenum shader_type, GLuint &out_shader);

    // Tests whether shader compiled successfully
    bool IsCompiled(GLuint shader);

    // Tests whether vertex/fragment programs link
    bool IsValidProgram();

private:
    // Store the shader object IDs
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint shader_program;
};

#endif // SHADER_H