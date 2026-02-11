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

#include <brenta/brenta.hpp>

#include <imgui_stdlib.h>

#include <iostream>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

using namespace brenta;

unsigned int update_shader(std::string fragment_source);

std::string fragment_color_red =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
  "}\0";
std::string fragment_color_animation =
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
std::string fragment_step =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform vec2 iResolution;\n\n"
  "void main()\n"
  "{\n"
  "  vec2 st = gl_FragCoord.xy/iResolution;\n"
  "  st.x = step(0.5, st.x);\n"
  "  FragColor = vec4(st.x, st.y, st.y, 1.0);\n"
  "}\0";
std::string fragment_smoothstep =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform vec2 iResolution;\n\n"
  "void main()\n"
  "{\n"
  "  vec2 st = gl_FragCoord.xy/iResolution;\n"
  "  st.x = smoothstep(0.3, 0.7, st.x);\n"
  "  FragColor = vec4(st.x, st.y, st.y, 1.0);\n"
  "}\0";
std::string fragment_mix =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform float iTime;\n\n"
  "void main()\n"
  "{\n"
  "   vec3 colorA = vec3(0.15, 0.14, 0.9);\n"
  "   vec3 colorB = vec3(1.0, 0.8, 0.2);\n\n"
  "   float pct = abs(sin(iTime));\n"
  "   vec3 color = mix(colorA, colorB, pct);\n"
  "   FragColor = vec4(color, 1.0);\n"
  "}\0";

std::string fragment_hsb =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform vec2 iResolution;\n\n"
  "vec3 rgb2hsb(vec3 c)\n"
  "{\n"
	"    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);\n"
  "    vec4 p = mix(vec4(c.bg, K.wz),\n"
  "                      vec4(c.gb, K.xy),\n"
  "                      step(c.b, c.g));\n"
  "    vec4 q = mix(vec4(p.xyw, c.r),\n"
  "                 vec4(c.r, p.yzx),\n"
  "                 step(p.x, c.r));\n"
  "    float d = q.x - min(q.w, q.y);\n"
  "    float e = 1.0e-10;\n"
  "    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),\n"
  "                d / (q.x + e),\n"
  "                q.x);\n"
  "}\n\n"
  "vec3 hsb2rgb(vec3 c)\n"
  "{\n"
  "   vec3 rgb = clamp(abs(mod(c.x*6.0 + vec3(0.0, 4.0, 2.0),\n"
  "                                           6.0)-3.0)-1.0,\n"
  "                                           0.0,\n"
  "                                           1.0);\n"
  "   rgb = rgb * rgb * (3.0-2.0*rgb);\n"
  "   return c.z * mix(vec3(1.0), rgb, c.y);\n"
  "}\n\n"
  "void main()\n"
  "{\n"
  "    vec2 st = gl_FragCoord.xy / iResolution;\n"
  "    vec3 color = vec3(0.0);\n"
  "    color = hsb2rgb(vec3(st.x, 1.0, st.y));\n"
  "    FragColor = vec4(color, 1.0);\n"
  "}\n";
std::string fragment_hsb_circle =
  "#version 330 core\n\n"
  "#define TWO_PI 6.28318530718\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform vec2 iResolution;\n\n"
  "vec3 hsb2rgb(vec3 c)\n"
  "{\n"
  "   vec3 rgb = clamp(abs(mod(c.x*6.0 + vec3(0.0, 4.0, 2.0),\n"
  "                                           6.0)-3.0)-1.0,\n"
  "                                           0.0,\n"
  "                                           1.0);\n"
  "   rgb = rgb * rgb * (3.0-2.0*rgb);\n"
  "   return c.z * mix(vec3(1.0), rgb, c.y);\n"
  "}\n\n"
  "void main()\n"
  "{\n"
  "    vec2 st = gl_FragCoord.xy / iResolution;\n"
  "    vec3 color = vec3(0.0);\n\n"
  "    vec2 toCenter = vec2(0.5) - st;\n"
  "    float angle = atan(toCenter.y, toCenter.x);\n"
  "    float radius = length(toCenter)*2.0;\n"
  "    color = hsb2rgb(vec3((angle / TWO_PI)+0.5, radius, 1.0));\n"
  "    FragColor = vec4(color, 1.0);\n"
  "}\n";
std::string fragment_circle =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform vec2 iResolution;\n"
  "uniform float iTime;\n\n"
  "void main()\n"
  "{\n"
  "    vec2 st = gl_FragCoord.xy/iResolution;\n\n"
  "    float pct = 0.0;\n"
  "    float radius = 0.2 * (abs(sin(iTime)) + abs(cos(iTime)));\n\n"
  "    pct = distance(st, vec2(0.5));\n"
  "    vec3 color = 1.0-vec3(step(radius, pct));\n"
  "    FragColor = vec4(color, 1.0);\n"
  "}";
std::string fragment_random =
  "#version 330 core\n\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n\n"
  "uniform vec2 iResolution;\n\n"
  "float random(vec2 st)\n"
  "{\n"
  "    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))*43758.5453123);\n"
  "}\n\n"
  "void main()\n"
  "{\n"
  "    vec2 st = gl_FragCoord.xy / iResolution.xy;\n"
  "    st *= 20;                          // Scale the coordinate system\n"
  "    vec2 ipos = floor(st);\n"
  "    vec2 fpos = fract(st);\n\n"
  "    vec3 color = vec3(random(ipos));\n"
  "    FragColor = vec4(color, 1.0);\n"
  "}\n";

int main(int argc, char** argv)
{
  //
  // Setup
  //
  
  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug)
          .file("/tmp/brenta-logs"))
    .with(Window::Builder()
          .title("shadertoy")
          .width(800)
          .height(600)
          .vsync()
          .debug())
    .with(Gl::Builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .with(Input::Builder())
    .with(Gui::Builder())
    .build();

  auto engine = Engine::managed();

  //
  // Variables
  //
  
  FrameBuffer fb(Window::get_width(), Window::get_height());
  
  //
  // Game loop
  //

  // A square
  float vertices[] = {// First Triangle
                      -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

                      // Second Triangle
                      -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f};
  Vao v;
  v.init();
  v.bind();
  Buffer vbo;
  vbo.init(GL_ARRAY_BUFFER);
  vbo.bind();
  vbo.copy_data(sizeof(vertices), vertices, GL_STATIC_DRAW);
  v.set_vertex_data(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  std::string  fragment_source = fragment_color_animation;
  std::string  prev_source     = fragment_source;
  unsigned int shader_program  = update_shader(fragment_source);

  // Load font
  ImGuiIO& io = ImGui::GetIO();
  ImFont* arial = io.Fonts->AddFontFromFileTTF("examples/assets/fonts/arial.ttf",
                                               30.0f);
  
  while(!Window::should_close())
  {
    if (Window::is_key_pressed(GLFW_KEY_ESCAPE))
      Window::close();

    if (prev_source != fragment_source)
    {
      prev_source = fragment_source;
      unsigned int ret = update_shader(fragment_source);
      if (ret > 0)
        shader_program = ret;
    }
    
    Gui::new_frame(&fb, "shadertoy");
    ImGui::PushFont(arial);
    ImGui::Begin("Settings");
    
    if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
    {
      if (ImGui::BeginTabItem("Shaders"))
      {
        const char* items[] = {
          "fragment_color_red",
          "fragment_color_animation",
          "fragment_step",
          "fragment_smoothstep",
          "fragment_mix",
          "fragment_hsb",
          "fragment_hsb_circle",
          "fragment_circle",
          "fragment_random"
        };
        static int item_selected_idx = 1;

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
                    case 0: fragment_source = fragment_color_red; break;
                    case 1: fragment_source = fragment_color_animation; break;
                    case 2: fragment_source = fragment_step; break;
                    case 3: fragment_source = fragment_smoothstep; break;
                    case 4: fragment_source = fragment_mix; break;
                    case 5: fragment_source = fragment_hsb; break;
                    case 6: fragment_source = fragment_hsb_circle; break;
                    case 7: fragment_source = fragment_circle; break;
                    case 8: fragment_source = fragment_random; break;
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
    
    Gl::clear();
    v.bind();

    glUseProgram(shader_program);
    
    // iTime
    GLint location = glGetUniformLocation(shader_program, "iTime");
    if (location != -1)
    {
      glUniform1f(location, Window::get_time().get_elapsed());
    }
    // iResolution
    location = glGetUniformLocation(shader_program, "iResolution");
    if (location != -1)
    {
      glUniform2f(location, Window::get_width(), Window::get_height());
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    v.unbind();
    fb.unbind();
    Gui::render();
    
    Window::poll_events();
    Window::swap_buffers();
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
