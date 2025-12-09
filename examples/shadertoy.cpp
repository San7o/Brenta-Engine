// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// shadertoy
// =========
//
// Play with shaders interactively
//

#ifdef BRENTA_USE_IMGUI

#include <brenta/engine.hpp>
#include <imgui_stdlib.h>

#include <iostream>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

using namespace brenta;

unsigned int update_shader(std::string fragment_source);

int main(int argc, char** argv)
{
  //
  // Setup
  //
  
  auto& engine = engine::builder()
    .subsystem(logger::builder()
               .level(oak::level::debug)
               .file("/tmp/brenta-logs"))
    .subsystem(window::builder()
               .title("shadertoy")
               .width(800)
               .height(600)
               .vsync())
    .subsystem(gl::builder()
               .blending()
               .cull_face()
               .multisample()
               .depth_test())
    .subsystem(input::builder())
    .subsystem(gui::builder())
    .build();
  
  auto ret = engine.initialize();
  if (!ret.has_value())
  {
    oak::error("Failed to initialize subsystem {}", ret.error());
    return 1;
  }

  //
  // Variables
  //
  
  types::framebuffer fb(window::get_width(), window::get_height());
  
  //
  // Game loop
  //

    // A square
  float vertices[] = {// First Triangle
                      -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

                      // Second Triangle
                      -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f};
  types::vao v;
  v.init();
  types::buffer vbo = types::buffer(GL_ARRAY_BUFFER);
  vbo.copy_data(sizeof(vertices), vertices, GL_STATIC_DRAW);
  v.set_vertex_data(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  std::string fragment_source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(ourColor, 1.0);\n"
    "}\0";
  std::string prev_source = fragment_source;
  unsigned int shader_program = update_shader(fragment_source);
  
  while(!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    if (prev_source != fragment_source)
    {
      prev_source = fragment_source;
      unsigned int ret = update_shader(fragment_source);
      if (ret != 0)
        shader_program = ret;
    }
    
    gui::new_frame(&fb);

    ImGui::Begin("Shader source");
    ImGui::InputTextMultiline("##source", &fragment_source,
                              ImVec2(1000, 1000),
                              ImGuiInputTextFlags_AllowTabInput);
    ImGui::End();

    fb.bind();
    
    gl::clear();
    v.bind();

    glUseProgram(shader_program);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    v.unbind();
    fb.unbind();
    gui::render();
    
    window::poll_events();
    window::swap_buffers();
  }

  //
  // Cleanup
  //
  
  ret = engine.terminate();
  if (!ret.has_value())
  {
    oak::error("Failed to terminate subsystem {}", ret.error());
    return 1;
  }
  return 0;
}

unsigned int update_shader(std::string fragment_source)
{

  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";
  unsigned int vertexShader;
  int success;
  char infoLog[512];
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    fprintf(stderr, "Vertex shader compilation failed: %s\n", infoLog);
    return 0;
  }

  const char *fragmentShaderSource = fragment_source.c_str();
  unsigned int fragmentShader;
  success = 1;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    fprintf(stderr, "Fragment shader compilation failed: %s\n", infoLog);
    return 0;
  }

  success = 1;
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    return 0;
  }

  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  
  return shaderProgram;
}

#endif // BRENTA_USE_IMGUI
