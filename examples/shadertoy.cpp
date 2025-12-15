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

std::string fragment_color1 =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform float iTime;\n"
  "uniform vec2 iResolution;\n\n"
  "void main()\n"
  "{\n"
  "  vec2 st = gl_FragCoord.xy/iResolution;\n"
  "  FragColor = vec4(abs(sin(iTime)) * st.x,\n"
  "                   abs(cos(iTime)) * st.y, 0.8, 1.0);\n"
  "}\0";
std::string fragment_color2 =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform float iTime;\n"
  "uniform vec2 iResolution;\n\n"
  "void main()\n"
  "{\n"
  "  vec2 st = gl_FragCoord.xy/iResolution;\n"
  "  FragColor = vec4(st.x, st.y, st.y, 1.0);\n"
  "}\0";


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

  std::string fragment_source = fragment_color1;
  std::string prev_source = fragment_source;
  unsigned int shader_program = update_shader(fragment_source);

  // Load font
  ImGuiIO& io = ImGui::GetIO();
  ImFont* arial = io.Fonts->AddFontFromFileTTF("examples/assets/fonts/arial.ttf",
                                               25.0f);
  
  while(!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    if (prev_source != fragment_source)
    {
      prev_source = fragment_source;
      unsigned int ret = update_shader(fragment_source);
      if (ret > 0)
        shader_program = ret;
    }
    
    gui::new_frame(&fb, "shadertoy");
    ImGui::PushFont(arial);
    ImGui::Begin("Settings");
    
    if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
    {
      if (ImGui::BeginTabItem("Shaders"))
      {
        const char* items[] = { "shader_color1", "shader_color2" };
        static int item_selected_idx = 0;

        static bool item_highlight = true;
        if (ImGui::BeginListBox(""))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    item_selected_idx = n;
                    switch (n)
                    {
                    case 0:
                      fragment_source = fragment_color1;
                      break;
                    case 1:
                      fragment_source = fragment_color2;
                      break;
                    }
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Edit"))
      {
        ImGui::InputTextMultiline("##source", &fragment_source,
                                  ImVec2(1000, 1000),
                                  ImGuiInputTextFlags_AllowTabInput);
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
    
    ImGui::PopFont();
    ImGui::End();

    fb.bind();
    
    gl::clear();
    v.bind();

    glUseProgram(shader_program);
    
    // iTime
    GLint location = glGetUniformLocation(shader_program, "iTime");
    if (location == -1)
    {
      ERROR("Uniform iTime not found in shader");
    }
    glUniform1f(location, time::get_current_time());
    // iResolution
    location = glGetUniformLocation(shader_program, "iResolution");
    if (location == -1)
    {
      ERROR("Uniform iResolution not found in shader");
    }
    glUniform2f(location, window::get_width(), window::get_height());
    
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

  const char *vertexShaderSource =
    "#version 330 core\n"
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
