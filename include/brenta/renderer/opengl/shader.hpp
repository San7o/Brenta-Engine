// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <tenno/vector.hpp>

#include <optional>
#include <string>
#include <filesystem>

namespace brenta
{

//
// Shader
// ------
//
// Shaders are programs that run on the GPU. They can be written in
// a high-level language and then compiled into instructions that
// the GPU understands. OpenGL uses the GLSL language which is similar
// to C in syntax.
//
// Therefore shaders can be compiled into a program, loaded and
// configured via special variables called "uniforms". They are
// special variables you can create in the shader that can be set
// from "outside" the shader, usually to pass som kind of state or
// configuration.
//
// This class provides a simple API to create shaders, load them and
// set uniforms.
//
class Shader
{
public:

  using Id   = unsigned int;

  enum class Type
  {
    Vertex,
    Fragment,
    Geometry,
    Compute,
  };

  class Object;
  class Builder;
  
  // Static API
  
  // Creates and compiles several shaders (into a shader "program")
  // Example:
  //     auto my_shader = Shader::create({
  //        { Shader::Type::Vertex, std::filesystem::path("shaders/shader.vs") },
  //        { Shader::Type::Fragment, std::filesystem::path("shaders/shader.fs") }});
  static std::optional<Shader>
  create(const tenno::vector<Shader::Object> &objects);

  // Set [feedback_varyings] to an array of CHchar* that specifies
  // the output attributes we want to capture into a buffer
  // For example:
  //    const GLchar* feedbackVaryings[] = { "outValue" };
  //
  // When using these shaders, you need to surround your draw call
  // with glBeginTransformFeedback() and glEndTransformFeedback()
  // like this:
  //
  //      my_shader.use();
  //      glBeginTransformFeedback(GL_POINTS); // Enter transform feedback mode
  //
  //      //...
  //      glDrawArrays(GL_POINTS, 0, num_particles);
  //      Gl::check_error();
  //      // ...
  //
  //      glEndTransformFeedback();         // Exit transform feedback mode
  static std::optional<Shader>
  create(const GLchar **feedback_varyings, int num_varyings,
         const tenno::vector<Shader::Object> &objects);

  // Set uniforms
  static bool set_bool(Shader::Id id, const std::string   &unif_name,
                       bool value);
  static bool set_int(Shader::Id id, const std::string    &unif_name,
                      int value);
  static bool set_float(Shader::Id id, const std::string  &unif_name,
                        float value);
  static bool set_float2(Shader::Id id, const std::string &unif_name,
                         float v1, float v2);
  static bool set_float3(Shader::Id id, const std::string &unif_name,
                         float v1, float v2, float v3);
  static bool set_mat4(Shader::Id id, const std::string   &unif_name,
                       glm::mat4 value);
  static bool set_vec3(Shader::Id id, const std::string   &unif_name,
                       float x, float y, float z);
  static bool set_vec3(Shader::Id id, const std::string   &unif_name,
                       glm::vec3 value);

  
  // Non static API
  
  Shader() = default;
  Shader(Shader::Id id) : id(id) {}
  Shader(Shader&& other)
  { this->id = other.id; other.id = 0; }
  ~Shader();

  Shader::Id        get_id() const;
  
  // Remember to call use() before setting uniforms and using this
  // shader
  bool              use();

  // Set uniforms
  bool set_bool(const std::string   &unif_name, bool  value);
  bool set_int(const std::string    &unif_name, int   value);
  bool set_float(const std::string  &unif_name, float value);
  bool set_float2(const std::string &unif_name, float v1, float v2);
  bool set_float3(const std::string &unif_name, float v1, float v2, float v3);
  bool set_mat4(const std::string   &unif_name, glm::mat4 value);
  bool set_vec3(const std::string   &unif_name, float x, float y, float z);
  bool set_vec3(const std::string   &unif_name, glm::vec3 value);

private:

  Shader::Id   id = 0;
  
  static bool
  compile_shaders([[maybe_unused]] tenno::vector<Shader::Id> &compiled);
  static bool compile_shaders(tenno::vector<Shader::Id> &compiled,
                              const tenno::vector<Shader::Object> &objects);
  static std::optional<Shader::Id>
  link_program(tenno::vector<Shader::Id>& compiled_shaders,
               const GLchar **feedback_varyings, int num_varyings);
  static void clean_compilation(tenno::vector<Shader::Id>& compiled_shader);
  static bool check_compile_errors(Shader::Id shader);  
  static bool check_link_errors(Shader::Id shader);

};

class Shader::Object
{
public:
  
  Type        type;
  std::string src;
  std::optional<std::filesystem::path> path;

  Object() = default;
  Object(Type type, const char* src)
    : type(type), src(std::string(src)), path({}) {}
  Object(Type type, const std::string &src)
    : type(type), src(src) {}
  Object(Type type, const std::filesystem::path &path);
  
private:

  static std::optional<std::string> read_file(const std::filesystem::path &path);

};

class Shader::Builder
{
public:

  Builder& object(const Shader::Object &obj);
  Builder& objects(const tenno::vector<Shader::Object> &objs);
  Builder& feedback(const GLchar **feedback_varyings, int num_varyings);

  // Add path to be watched for hot-reloading
  Builder &watch(const std::filesystem::path &path);
 
  std::optional<Shader> build();
  tenno::vector<std::filesystem::path> get_watch_paths() const;
  
private:

  int num_varyings = 0;
  const GLchar **feedback_varyings;
  tenno::vector<Object> objs;

  tenno::vector<std::filesystem::path> watch_paths = {};
  
};

} // namespace brenta
