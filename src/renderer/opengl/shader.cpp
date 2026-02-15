// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/shader.hpp>

#include <sstream>

using namespace brenta;

std::unordered_map<Shader::Name, Shader::Id> Shader::shaders;

std::optional<Shader>
Shader::create(const Shader::Name &shader_name,
               const std::vector<Shader::Object> &objects)
{
  std::vector<Shader::Id> compiled_shaders = {};
  if (!compile_shaders(compiled_shaders, objects))
  {
    ERROR("shader: error compiling shader {}", shader_name);
    return {};
  }

  std::optional<Shader::Id> id =
    Shader::link_program(compiled_shaders, nullptr, 0);
  if (!id) return {};
  
  Shader::clean_compilation(compiled_shaders);
  Shader::shaders.insert({shader_name, *id});
  return Shader(*id, shader_name);
}

std::optional<Shader>
Shader::create(const GLchar **feedback_varyings, int num_varyings,
               const Shader::Name &shader_name,
               const std::vector<Shader::Object> &objects)
{
  std::vector<Shader::Id> compiled_shaders = {};
  if (!compile_shaders(compiled_shaders, objects))
  {
    ERROR("shader: error compiling shader {}", shader_name);
    return {};
  }

  std::optional<Shader::Id> id =
    Shader::link_program(compiled_shaders, feedback_varyings, num_varyings);
  if (!id) return {};
  
  Shader::clean_compilation(compiled_shaders);
  Shader::shaders.insert({shader_name, *id});
  return Shader(*id, shader_name);
}

bool Shader::compile_shaders(std::vector<Shader::Id> &compiled,
                             const std::vector<Shader::Object> &objects)
{
  for (auto& obj : objects)
  {
    GLenum shader_type_gl;
    switch(obj.type)
    {
    case Shader::Type::Fragment:  shader_type_gl  = GL_FRAGMENT_SHADER; break;
    case Shader::Type::Vertex:    shader_type_gl  = GL_VERTEX_SHADER; break;
    case Shader::Type::Geometry:  shader_type_gl  = GL_GEOMETRY_SHADER; break;
    case Shader::Type::Compute:   shader_type_gl  = GL_COMPUTE_SHADER; break;
    default:                      shader_type_gl  = 0; break;
    }
    
    unsigned int shader = glCreateShader(shader_type_gl);
    const GLchar *src = obj.src.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    if (!Shader::check_compile_errors(shader))
      return false;

    compiled.push_back(shader);
  }
  return true;
}

bool Shader::compile_shaders([[maybe_unused]] std::vector<Shader::Id> &compiled)
{
  return true;
}

std::optional<Shader::Id>
Shader::link_program(std::vector<Shader::Id>& compiled_shaders,
                     const GLchar **feedback_varyings, int num_varyings)
{
  Shader::Id id = glCreateProgram();
  std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                [&id](auto shader) { glAttachShader(id, shader); });

  if (feedback_varyings != nullptr)
    glTransformFeedbackVaryings(id, num_varyings, feedback_varyings,
                                GL_INTERLEAVED_ATTRIBS);
  
  glLinkProgram(id);
  if (!Shader::check_link_errors(id))
    return {};
  return id;
}

void Shader::clean_compilation(std::vector<Shader::Id>& compiled_shaders)
{
  std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                [](auto shader) { glDeleteShader(shader); });
 return;
}

std::optional<Shader> Shader::get_shader(Shader::Name shader_name)
{
  if (Shader::shaders.find(shader_name) == Shader::shaders.end())
  {
    return {};
  }
  return Shader(Shader::shaders.at(shader_name), shader_name);
}

Shader::Id Shader::get_id() const
{
  return this->id;
}

Shader::Name Shader::get_name() const
{
  return this->name;
}

// Use/activate the shader
bool Shader::use()
{
  glUseProgram(this->get_id());
  
  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::use: error using shader {}: {}", this->get_name(), err);
    return false;
  }
  return true;
}

bool Shader::set_bool(const GLchar *unif_name, bool value)
{
  GLuint program = this->get_id();
  GLint location = glGetUniformLocation(program, unif_name);

  if (location == -1)
  {
    ERROR("Shader::set_bool: uniform '{}' not found in shader '{}'",
          unif_name, this->get_name());
    return false;
  }
  
  glUniform1i(location, (int) value);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::set_bool: error setting bool value for shader {} with name {}: {}",
          this->get_name(), unif_name, err);
    return false;
  }
  return true;
}

bool Shader::set_int(const GLchar *unif_name, int value)
{
  GLuint program = this->get_id();
  GLint location = glGetUniformLocation(program, unif_name);

  if (location == -1)
  {
    ERROR("Shader::set_int: uniform '{}' not found in shader '{}'",
          unif_name, this->get_name());
    return false;
  }

  glUniform1i(location, value);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::set_int: error setting int value for shader '{}' with name '{}'",
          this->get_name(), unif_name, err);
    return false;
  }
  return true;
}

bool Shader::set_float(const GLchar *unif_name, float value)
{
  GLuint program = this->get_id();
  GLint location = glGetUniformLocation(program, unif_name);

  if (location == -1)
  {
    ERROR("Shader::set_float: uniform '{}' not found in shader '{}'",
          unif_name, this->get_name());
    return false;
  }

  glUniform1f(location, value);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::set_float: error setting float value for shader '{}' with name '{}': {}",
          this->get_name(), unif_name, err);
    return false;
  }
  return true;
}

bool Shader::set_float2(const GLchar *unif_name, float v1, float v2)
{
  GLuint program = this->get_id();
  GLint location = glGetUniformLocation(program, unif_name);

  if (location == -1)
  {
    ERROR("Shader::set_float2: uniform '{}' not found in shader '{}'",
          unif_name, this->get_name());
    return false;
  }

  glUniform2f(location, v1, v2);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::set_float2: error setting float uniform for shader '{}' with name '{}': {}",
          this->get_name(), unif_name, err);
    return false;
  }
  return true;
}

bool Shader::set_float3(const GLchar *unif_name, float v1, float v2, float v3)
{
  GLuint program = this->get_id();
  GLint location = glGetUniformLocation(program, unif_name);

  if (location == -1)
  {
    ERROR("Shader::set_float3: uniform '{}' not found in shader '{}'",
          unif_name, this->get_name());
    return false;
  }

  glUniform3f(location, v1, v2, v3);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::set_float3: error setting float value for shader '{}' with name '{}': {}",
          this->get_name(), unif_name, err);
    return false;
  }
  return true;
}

bool Shader::set_mat4(const GLchar *unif_name, glm::mat4 value)
{
  GLuint program = this->get_id();
  GLint location = glGetUniformLocation(program, unif_name);

  if (location == -1)
  {
    ERROR("Shader::set_mat4: uniform '{}' not found in shader '{}'",
          unif_name, this->get_name());
    return false;
  }
  
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::set_mat4: error setting mat4 value for shader '{}' with name '{}': {}",
          this->get_name(), unif_name, err);
    return false;
  }
  return true;
}

bool Shader::set_vec3(const GLchar *unif_name, float x, float y, float z)
{
  GLuint program = this->get_id();
  GLint location = glGetUniformLocation(program, unif_name);

  if (location == -1)
  {
    ERROR("Shader::set_vec3: uniform '{}' not found in shader '{}'",
          unif_name, this->get_name());
    return false;
  }
  
  glUniform3f(location, x, y, z);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::set_vec3: error setting vec3 value for shader '{}' with name '{}': {}",
          this->get_name(), unif_name, err);
    return false;
  }
  return true;
}

bool Shader::set_vec3(const GLchar *unif_name, glm::vec3 value)
{
  GLuint program = this->get_id();
  GLint location = glGetUniformLocation(program, unif_name);

  if (location == -1)
  {
    ERROR("Shader::set_vec3: uniform '{}' not found in shader '{}'",
          unif_name, this->get_name());
    return false;
  }
  
  glUniform3f(location, value.x, value.y, value.z);

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    ERROR("Shader::set_vec3: error setting vec3 value for shader '{}' with name '{}': {}",
          this->get_name(), unif_name, err);
    return false;
  }
  return true;
}

bool Shader::check_compile_errors(Shader::Id shader)
{
  int success;
  char infoLog[1024];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success) return true;
  
  std::stringstream out;
  glGetShaderInfoLog(shader, 1024, NULL, infoLog);
  out << "shader: compilation error: " << infoLog;
  ERROR("{}", out.str());
  return false;
}

bool Shader::check_link_errors(Shader::Id shader)
{
  int success;
  char infoLog[1024];
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (success) return true;
  
  std::stringstream out;
  glGetProgramInfoLog(shader, 1024, NULL, infoLog);
  out << "shader: program linking error: " << infoLog;
  ERROR("{}", out.str());
  return false;
}

//
// Object
//

Shader::Object::Object(Type type, const std::filesystem::path &path)
{
  this->type = type;
  auto src = Object::read_file(path);
  if (!src)
  {
    this->src = "";
  }
  else
  {
    this->src = *src;
  }
  return;
}

std::optional<std::string>
Shader::Object::read_file(const std::filesystem::path &path)
{
  std::string code;
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    file.open(path);
    if (!file.is_open()) throw "Cannot open file";
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    code = stream.str();
  }
  catch (std::ifstream::failure &e)
  {
    ERROR("shader: error reading shader file: {}", path.string());
    return {};
  }

  if (code.empty())
  {
    ERROR("shader: file is empty: {}", path.string());
    return {};
  }

  return code;
}
