#include "Shader.h"
#include "Texture.h"
#include "SDL2/SDL.h"
#include <fstream>
#include <sstream>

Shader::Shader()
    : shader_program(0), vertex_shader(0), fragment_shader(0) {}

Shader::~Shader() {}

bool Shader::Load(const std::string &_vertex_name, const std::string &_fragmet_name)
{
  // Compile vertex and pixel shaders
  if (!CompileShader(_vertex_name, GL_VERTEX_SHADER, vertex_shader) ||
      !CompileShader(_fragmet_name, GL_FRAGMENT_SHADER, fragment_shader))
  {
    return false;
  }

  // Now create a shader program that
  // links together the vertex/frag shaders
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  // Verify that the program linked successfully
  if (!IsValidProgram())
  {
    return false;
  }

  return true;
}

void Shader::Unload()
{
  // Delete the program/shaders
  glDeleteProgram(shader_program);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void Shader::Set_Active()
{
  // Set this program as the active one
  glUseProgram(shader_program);
}

void Shader::Set_Matrix_Uniform(const char *_name, const Matrix4 &_matrix)
{
  // Find the uniform by this name
  GLuint loc = glGetUniformLocation(shader_program, _name);
  // Send the matrix data to the uniform
  glUniformMatrix4fv(loc, 1, GL_TRUE, _matrix.GetAsFloatPtr());
}

void Shader::Set_Vector_Uniform(const char *_name, const Vector3 &vector)
{
  GLuint location = glGetUniformLocation(shader_program, _name);
  // Send the vector data
  glUniform3fv(location, 1, vector.GetAsFloatPtr());
}

void Shader::Set_Float_Uniform(const char *_name, float _value)
{
  GLuint loc = glGetUniformLocation(shader_program, _name);
  // Send the float data
  glUniform1f(loc, _value);
}

bool Shader::CompileShader(const std::string &file_name, GLenum shader_type, GLuint &out_shader)
{
  // Open file
  std::ifstream shader_file(file_name);
  if (shader_file.is_open())
  {
    // Read all the text into a string
    std::stringstream sstream;
    sstream << shader_file.rdbuf();
    std::string contents = sstream.str();
    const char *contentsChar = contents.c_str();

    // Create a shader of the specified type
    out_shader = glCreateShader(shader_type);
    // Set the source characters and try to compile
    glShaderSource(out_shader, 1, &(contentsChar), nullptr);
    glCompileShader(out_shader);

    if (!IsCompiled(out_shader))
    {
      SDL_Log("Failed to compile shader %s", file_name.c_str());
      return false;
    }
  }
  else
  {
    SDL_Log("Shader file not found: %s", file_name.c_str());
    return false;
  }

  return true;
}

bool Shader::IsCompiled(GLuint shader)
{
  GLint status;
  // Query the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE)
  {
    char buffer[512];
    memset(buffer, 0, 512);
    glGetShaderInfoLog(shader, 511, nullptr, buffer);
    SDL_Log("GLSL Compile Failed:\n%s", buffer);
    return false;
  }

  return true;
}

bool Shader::IsValidProgram()
{

  GLint status;
  // Query the link status
  glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
  if (status != GL_TRUE)
  {
    char buffer[512];
    memset(buffer, 0, 512);
    glGetProgramInfoLog(shader_program, 511, nullptr, buffer);
    SDL_Log("GLSL Link Status:\n%s", buffer);
    return false;
  }

  return true;
}