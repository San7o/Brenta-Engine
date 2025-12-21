// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/shader.hpp>

#include <sstream>

using namespace brenta;

std::unordered_map<shader::name_t, unsigned int> shader::shaders;

unsigned int shader::get_id(shader::name_t shader_name)
{
  if (shader::shaders.find(shader_name) == shader::shaders.end())
  {
    return 0;
  }
  return shader::shaders.at(shader_name);
}

// Use/activate the shader
bool shader::use(shader::name_t shader_name)
{
  glUseProgram(shader::get_id(shader_name));
  
  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("shader::use: error using shader {}: {}", shader_name, err);
    return false;
  }
  return true;
}

bool shader::set_bool(shader::name_t shader_name,
                      const GLchar *name,
                      bool value)
{
  GLuint program = shader::get_id(shader_name);
  GLint location = glGetUniformLocation(program, name);

  if (location == -1)
  {
    ERROR("shader::set_bool: uniform '{}' not found in shader '{}'",
          name, shader_name);
    return false;
  }
  
  glUniform1i(location, (int) value);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("shader::set_bool: error setting bool value for shader {} with name {}: {}",
          shader_name, name, err);
    return false;
  }
  return true;
}

bool shader::set_int(shader::name_t shader_name,
                     const GLchar *name,
                     int value)
{
  GLuint program = shader::get_id(shader_name);
  GLint location = glGetUniformLocation(program, name);

  if (location == -1)
  {
    ERROR("shader::set_int: uniform '{}' not found in shader '{}'",
          name, shader_name);
    return false;
  }

  glUniform1i(location, value);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("shader::set_int: error setting int value for shader '{}' with name '{}'",
          shader_name, name, err);
    return false;
  }
  return true;
}

bool shader::set_float(shader::name_t shader_name,
                       const GLchar *name, float value)
{
  GLuint program = shader::get_id(shader_name);
  GLint location = glGetUniformLocation(program, name);

  if (location == -1)
  {
    ERROR("shader::set_float: uniform '{}' not found in shader '{}'",
          name, shader_name);
    return false;
  }

  glUniform1f(location, value);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("shader::set_float: error setting float value for shader '{}' with name '{}': {}",
          shader_name, name, err);
    return false;
  }
  return true;
}

bool shader::set_mat4(shader::name_t shader_name,
                      const GLchar *name,
                      glm::mat4 value)
{
  GLuint program = shader::get_id(shader_name);
  GLint location = glGetUniformLocation(program, name);

  if (location == -1)
  {
    ERROR("shader::set_mat4: uniform '{}' not found in shader '{}'",
          name, shader_name);
    return false;
  }
  
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("shader::set_mat4: error setting mat4 value for shader '{}' with name '{}': {}",
          shader_name, name, err);
    return false;
  }
  return true;
}

bool shader::set_vec3(shader::name_t shader_name,
                      const GLchar *name,
                      float x, float y, float z)
{
  GLuint program = shader::get_id(shader_name);
  GLint location = glGetUniformLocation(program, name);

  if (location == -1)
  {
    ERROR("shader::set_vec3: uniform '{}' not found in shader '{}'",
          name, shader_name);
    return false;
  }
  
  glUniform3f(location, x, y, z);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("shader::set_vec3: error setting vec3 value for shader '{}' with name '{}': {}",
          shader_name, name, err);
    return false;
  }
  return true;
}

bool shader::set_vec3(shader::name_t shader_name,
                      const GLchar *name,
                      glm::vec3 value)
{
  GLuint program = shader::get_id(shader_name);
  GLint location = glGetUniformLocation(program, name);

  if (location == -1)
  {
    ERROR("shader::set_vec3: uniform '{}' not found in shader '{}'",
          name, shader_name);
    return false;
  }
  
  glUniform3f(location, value.x, value.y, value.z);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("shader::set_vec3: error setting vec3 value for shader '{}' with name '{}': {}",
          shader_name, name, err);
    return false;
  }
  return true;
}

bool shader::check_compile_errors(unsigned int shader, std::string type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      std::stringstream out;
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      out << "shader: compilation error of type: " << type << "\n"
          << infoLog;
      ERROR("{}", out.str());
      return false;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      std::stringstream out;
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      out << "shader: program linking error of type: " << type << "\n"
          << infoLog;
      ERROR("{}", out.str());
      return false;
    }
  }
  return true;
}
