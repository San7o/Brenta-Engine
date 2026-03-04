// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Helper functions to interact with OpenGL.
//

#pragma once

#include <brenta/subsystem.hpp>
#include <brenta/color.hpp>

#include <glad/glad.h> // OpenGL driver

#include <expected>
#include <string>

#define check_error() Gl::_check_error(__FILE__, __LINE__)

namespace brenta
{

//
// OpenGL helper functions
// -----------------------
//
// This class contains static helper functions to interact with OpenGL.
//
class Gl : public Subsystem
{
public:

  class Config;
  class Builder;

  static const GLboolean True;
  static const GLboolean False;

  enum Type {
    Byte          = GL_BYTE,
    UnsignedByte  = GL_UNSIGNED_BYTE,
    Short         = GL_SHORT,
    UnsignedShort = GL_UNSIGNED_SHORT,
    Int           = GL_INT,
    UnsignedInt   = GL_UNSIGNED_INT,
    Float         = GL_FLOAT,
  };
  
  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate()  override;
  std::string name() override;
  bool is_initialized() override;

  // Member functions
  
  static Gl &instance();
  
  static void set_poligon_mode(GLboolean enable);
  static void set_viewport(int x, int y, int width, int height);
  static void set_color(const Color &color);
  static void draw_arrays(GLenum mode, int first, int count);
  static void draw_elements(GLenum mode, int count, GLenum type,
                            const void *indices);
  static void clear();
  
  static GLenum _check_error(const char *file, int line);

private:
  
  static bool         initialized;
  static Gl::Config   init_config;

  // Private constructors / destructors for singleton  
  Gl()  = default;
  ~Gl() = default;
  
};

struct Gl::Config
{
public:
  bool enable_blending           = false;
  bool enable_backface_culling   = false;
  bool enable_multisample        = false;
  bool enable_depth_test         = false;
};
  
class Gl::Builder : public Subsystem::Builder
{
private:

  Gl::Config conf = {};
  
public:

  Builder()  = default;
  ~Builder() = default;

  Builder &blending();
  Builder &backface_culling();
  Builder &multisample();
  Builder &depth_test();

  Subsystem &build() override;
  
};
  
} // namespace brenta
