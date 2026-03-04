// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/drivers/glfw.hpp>
#include <brenta/logger.hpp>
#include <brenta/input.hpp>

#include <array>

using namespace brenta;

std::expected<void, std::string>
GlfwDriver::initialize(const Window::Config &conf)
{
  if (glfwInit() == GLFW_FALSE)
    return std::unexpected("Glfw: failed to initialize driver");

  this->set_context_version(3, 3);
  this->use_core_profile();

  if (conf.msaa)
  {
    glfwWindowHint(GLFW_SAMPLES, 4);
    INFO("Glfw: enabled MSAA");
  }
  else
  {
    INFO("Glfw: disabled MSAA");
  }

  if (!conf.vsync)
  {
    glfwSwapInterval(0);
    INFO("Glfw: disabled VSync");
  }
  else
  {
    INFO("Glfw: enabled VSync");
  }

  if (conf.debug)
  {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    INFO("Glfw: enabled OPENGL_DEBUG_CONTEXT");
  }

#ifdef __APPLE__
  this->set_hints_apple();
#endif

  this->create_window(conf.width, conf.height, conf.title);
  this->make_context_current();
  this->set_mouse_capture(conf.capture_mouse);
  this->set_size_callback((void*)framebuffer_size_callback);
  this->set_key_callback((void*)glfw_key_callback);
  this->set_mouse_callback((void*)mouse_callback);


  INFO("Glfw: driver initialized")
  return {};
}

std::expected<void, std::string> GlfwDriver::terminate()
{
  if (!this->window)
  {
    ERROR("Glfw: tried to terminate driver, but it was never initialized");
    return std::unexpected("Uninitialized");
  }
  glfwDestroyWindow(this->window);
  glfwTerminate();
  INFO("Glfw: driver terminated");
  return {};
}

bool GlfwDriver::should_close()
{
  return glfwWindowShouldClose(this->window);
}

void GlfwDriver::set_width_height(int width, int height)
{
  this->width = width;
  this->height = height;
  return;
}

bool GlfwDriver::is_key_pressed(Key key)
{
  int glfw_key = this->key_to_glfw(key);
  return glfwGetKey(this->window, glfw_key) == GLFW_PRESS;
}

void GlfwDriver::set_dimensions(int width, int height)
{
  this->width  = width;
  this->height = height;
  return;
}

Time GlfwDriver::get_time()
{
  return this->time;
}

Window::WindowHandle GlfwDriver::get_window()
{
  return this->window;
}

Window::ProcHandle GlfwDriver::get_proc_address()
{
  return (void*)glfwGetProcAddress;
}

int GlfwDriver::get_width()
{
  return this->width;
}

int GlfwDriver::get_height()
{
  return this->height;
}

void GlfwDriver::update_dimensions()
{
  glfwGetWindowSize(this->window, &this->width, &this->height);
  return;
}

void GlfwDriver::mouse_callback([[maybe_unused]] GLFWwindow *window,
                          double xpos,
                          double ypos)
{
  Input::mouse_callback(xpos, ypos);
  return;
}


void GlfwDriver::set_mouse_callback(void* callback)
{
  glfwSetCursorPosCallback(this->window, (GLFWcursorposfun) callback);
  DEBUG("Glfw: set mouse callback");
  return;
}

void GlfwDriver::set_size_callback(void* callback)
{
  glfwSetFramebufferSizeCallback(this->window, (GLFWframebuffersizefun) callback);
  DEBUG("Glfw: set framebuffer size callback");
  return;
}

void GlfwDriver::set_mouse_capture(bool is_captured)
{
  if (is_captured)
  {
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    INFO("Glfw: mouse capture enabled");
  }
  else
  {
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    INFO("Glfw: mouse capture disabled");
  }
  return;
}

void GlfwDriver::close()
{
  glfwSetWindowShouldClose(this->window, GLFW_TRUE);
  return;
}

void GlfwDriver::swap_buffers()
{
  this->time.update(glfwGetTime());
  glfwSwapBuffers(this->window);
  return;
}

void GlfwDriver::poll_events()
{
  glfwPollEvents();
  return;
}

void GlfwDriver::set_context_version(int major, int minor)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

  INFO("Glfw: set context to OpenGL version: {}.{}", major, minor);
  return;
}

void GlfwDriver::glfw_key_callback([[maybe_unused]] GLFWwindow *window,
                             int glfw_key,
                             [[maybe_unused]] int scancode,
                             int glfw_action,
                             int glfw_mods)
{
  auto key = GlfwDriver::glfw_to_key(glfw_key);

  KeyAction action;
  switch(glfw_action)
  {
  case GLFW_RELEASE: action = KeyAction::Release; break;
  case GLFW_PRESS:   action = KeyAction::Press; break;
  case GLFW_REPEAT:  action = KeyAction::Repeat; break;
  case GLFW_KEY_UNKNOWN:
  default:
    action = KeyAction::Unknown;
    break;
  }

  KeyMods mods= {};
  if (glfw_mods & GLFW_MOD_SHIFT)
    mods.set_shift();
  if (glfw_mods & GLFW_MOD_CONTROL)
    mods.set_control();
  if (glfw_mods & GLFW_MOD_ALT)
    mods.set_alt();
  if (glfw_mods & GLFW_MOD_SUPER)
    mods.set_super();
  if (glfw_mods & GLFW_MOD_CAPS_LOCK)
    mods.set_caps_lock();
  if (glfw_mods & GLFW_MOD_NUM_LOCK)
    mods.set_num_lock();
  
  Input::key_callback(key, action, mods);
  return;
}

void GlfwDriver::set_key_callback(void* callback)
{
  glfwSetKeyCallback(this->window, (GLFWkeyfun) callback);
  DEBUG("Glfw: set key callback");
  return;
}

void GlfwDriver::set_mouse_pos_callback(void* callback)
{
  glfwSetCursorPosCallback(this->window, (GLFWcursorposfun) callback);
  DEBUG("Glfw: set mouse pos");
  return;
}

void GlfwDriver::create_window(int width, int height, const std::string &title)
{
  this->window = glfwCreateWindow(width, height, title.c_str(),
                                  NULL, NULL);
  if (this->window == NULL)
  {
    ERROR("Glfw: failed to create window");
    Window::instance().terminate();
  }
  this->width = width;
  this->height = height;
  return;
}

void GlfwDriver::make_context_current()
{
  glfwMakeContextCurrent(this->window);
  return;
}

void GlfwDriver::framebuffer_size_callback([[maybe_unused]] GLFWwindow *window,
                                     [[maybe_unused]] int width,
                                     [[maybe_unused]] int height)
{
  if (Gl::instance().is_initialized())
    glViewport(0, 0, width, height);

  Window::get_driver()->set_width_height(width, height);

  DEBUG("Glfw: size changed {}x{}", width, height);
  return;
}

int GlfwDriver::key_to_glfw(Key key)
{
  static const auto key_map = []() {
    std::array<int, static_cast<size_t>(Key::Menu) + 1> map;
        
    map.fill(GLFW_KEY_UNKNOWN);

    map[static_cast<size_t>(Key::Space)]         = GLFW_KEY_SPACE;
    map[static_cast<size_t>(Key::Apostrophe)]    = GLFW_KEY_APOSTROPHE;
    map[static_cast<size_t>(Key::Comma)]         = GLFW_KEY_COMMA;
    map[static_cast<size_t>(Key::Minus)]         = GLFW_KEY_MINUS;
    map[static_cast<size_t>(Key::Period)]        = GLFW_KEY_PERIOD;
    map[static_cast<size_t>(Key::Slash)]         = GLFW_KEY_SLASH;
    map[static_cast<size_t>(Key::D0)]            = GLFW_KEY_0;
    map[static_cast<size_t>(Key::D1)]            = GLFW_KEY_1;
    map[static_cast<size_t>(Key::D2)]            = GLFW_KEY_2;
    map[static_cast<size_t>(Key::D3)]            = GLFW_KEY_3;
    map[static_cast<size_t>(Key::D4)]            = GLFW_KEY_4;
    map[static_cast<size_t>(Key::D5)]            = GLFW_KEY_5;
    map[static_cast<size_t>(Key::D6)]            = GLFW_KEY_6;
    map[static_cast<size_t>(Key::D7)]            = GLFW_KEY_7;
    map[static_cast<size_t>(Key::D8)]            = GLFW_KEY_8;
    map[static_cast<size_t>(Key::D9)]            = GLFW_KEY_9;
    map[static_cast<size_t>(Key::Semicolon)]     = GLFW_KEY_SEMICOLON;
    map[static_cast<size_t>(Key::Equal)]         = GLFW_KEY_EQUAL;
    map[static_cast<size_t>(Key::A)]             = GLFW_KEY_A;
    map[static_cast<size_t>(Key::B)]             = GLFW_KEY_B;
    map[static_cast<size_t>(Key::C)]             = GLFW_KEY_C;
    map[static_cast<size_t>(Key::D)]             = GLFW_KEY_D;
    map[static_cast<size_t>(Key::E)]             = GLFW_KEY_E;
    map[static_cast<size_t>(Key::F)]             = GLFW_KEY_F;
    map[static_cast<size_t>(Key::G)]             = GLFW_KEY_G;
    map[static_cast<size_t>(Key::H)]             = GLFW_KEY_H;
    map[static_cast<size_t>(Key::I)]             = GLFW_KEY_I;
    map[static_cast<size_t>(Key::J)]             = GLFW_KEY_J;
    map[static_cast<size_t>(Key::K)]             = GLFW_KEY_K;
    map[static_cast<size_t>(Key::L)]             = GLFW_KEY_L;
    map[static_cast<size_t>(Key::M)]             = GLFW_KEY_M;
    map[static_cast<size_t>(Key::N)]             = GLFW_KEY_N;
    map[static_cast<size_t>(Key::O)]             = GLFW_KEY_O;
    map[static_cast<size_t>(Key::P)]             = GLFW_KEY_P;
    map[static_cast<size_t>(Key::Q)]             = GLFW_KEY_Q;
    map[static_cast<size_t>(Key::R)]             = GLFW_KEY_R;
    map[static_cast<size_t>(Key::S)]             = GLFW_KEY_S;
    map[static_cast<size_t>(Key::T)]             = GLFW_KEY_T;
    map[static_cast<size_t>(Key::U)]             = GLFW_KEY_U;
    map[static_cast<size_t>(Key::V)]             = GLFW_KEY_V;
    map[static_cast<size_t>(Key::W)]             = GLFW_KEY_W;
    map[static_cast<size_t>(Key::X)]             = GLFW_KEY_X;
    map[static_cast<size_t>(Key::Y)]             = GLFW_KEY_Y;
    map[static_cast<size_t>(Key::Z)]             = GLFW_KEY_Z;
    map[static_cast<size_t>(Key::LeftBracket)]   = GLFW_KEY_LEFT_BRACKET;
    map[static_cast<size_t>(Key::Backslash)]     = GLFW_KEY_BACKSLASH;
    map[static_cast<size_t>(Key::RightBracket)]  = GLFW_KEY_RIGHT_BRACKET;
    map[static_cast<size_t>(Key::GraveAccent)]   = GLFW_KEY_GRAVE_ACCENT;
    map[static_cast<size_t>(Key::World1)]        = GLFW_KEY_WORLD_1;
    map[static_cast<size_t>(Key::World2)]        = GLFW_KEY_WORLD_2;

    // Function keys
    map[static_cast<size_t>(Key::Escape)]        = GLFW_KEY_ESCAPE;
    map[static_cast<size_t>(Key::Enter)]         = GLFW_KEY_ENTER;
    map[static_cast<size_t>(Key::Tab)]           = GLFW_KEY_TAB;
    map[static_cast<size_t>(Key::Backspace)]     = GLFW_KEY_BACKSPACE;
    map[static_cast<size_t>(Key::Insert)]        = GLFW_KEY_INSERT;
    map[static_cast<size_t>(Key::Delete)]        = GLFW_KEY_DELETE;
    map[static_cast<size_t>(Key::Right)]         = GLFW_KEY_RIGHT;
    map[static_cast<size_t>(Key::Left)]          = GLFW_KEY_LEFT;
    map[static_cast<size_t>(Key::Down)]          = GLFW_KEY_DOWN;
    map[static_cast<size_t>(Key::Up)]            = GLFW_KEY_UP;
    map[static_cast<size_t>(Key::PageUp)]        = GLFW_KEY_PAGE_UP;
    map[static_cast<size_t>(Key::PageDown)]      = GLFW_KEY_PAGE_DOWN;
    map[static_cast<size_t>(Key::Home)]          = GLFW_KEY_HOME;
    map[static_cast<size_t>(Key::End)]           = GLFW_KEY_END;
    map[static_cast<size_t>(Key::CapsLock)]      = GLFW_KEY_CAPS_LOCK;
    map[static_cast<size_t>(Key::ScrollLock)]    = GLFW_KEY_SCROLL_LOCK;
    map[static_cast<size_t>(Key::NumLock)]       = GLFW_KEY_NUM_LOCK;
    map[static_cast<size_t>(Key::PrintScreen)]   = GLFW_KEY_PRINT_SCREEN;
    map[static_cast<size_t>(Key::Pause)]         = GLFW_KEY_PAUSE;
    map[static_cast<size_t>(Key::F1)]            = GLFW_KEY_F1;
    map[static_cast<size_t>(Key::F2)]            = GLFW_KEY_F2;
    map[static_cast<size_t>(Key::F3)]            = GLFW_KEY_F3;
    map[static_cast<size_t>(Key::F4)]            = GLFW_KEY_F4;
    map[static_cast<size_t>(Key::F5)]            = GLFW_KEY_F5;
    map[static_cast<size_t>(Key::F6)]            = GLFW_KEY_F6;
    map[static_cast<size_t>(Key::F7)]            = GLFW_KEY_F7;
    map[static_cast<size_t>(Key::F8)]            = GLFW_KEY_F8;
    map[static_cast<size_t>(Key::F9)]            = GLFW_KEY_F9;
    map[static_cast<size_t>(Key::F10)]           = GLFW_KEY_F10;
    map[static_cast<size_t>(Key::F11)]           = GLFW_KEY_F11;
    map[static_cast<size_t>(Key::F12)]           = GLFW_KEY_F12;
    map[static_cast<size_t>(Key::F13)]           = GLFW_KEY_F13;
    map[static_cast<size_t>(Key::F14)]           = GLFW_KEY_F14;
    map[static_cast<size_t>(Key::F15)]           = GLFW_KEY_F15;
    map[static_cast<size_t>(Key::F16)]           = GLFW_KEY_F16;
    map[static_cast<size_t>(Key::F17)]           = GLFW_KEY_F17;
    map[static_cast<size_t>(Key::F18)]           = GLFW_KEY_F18;
    map[static_cast<size_t>(Key::F19)]           = GLFW_KEY_F19;
    map[static_cast<size_t>(Key::F20)]           = GLFW_KEY_F20;
    map[static_cast<size_t>(Key::F21)]           = GLFW_KEY_F21;
    map[static_cast<size_t>(Key::F22)]           = GLFW_KEY_F22;
    map[static_cast<size_t>(Key::F23)]           = GLFW_KEY_F23;
    map[static_cast<size_t>(Key::F24)]           = GLFW_KEY_F24;
    map[static_cast<size_t>(Key::F25)]           = GLFW_KEY_F25;

    // Keypad
    map[static_cast<size_t>(Key::KP0)]           = GLFW_KEY_KP_0;
    map[static_cast<size_t>(Key::KP1)]           = GLFW_KEY_KP_1;
    map[static_cast<size_t>(Key::KP2)]           = GLFW_KEY_KP_2;
    map[static_cast<size_t>(Key::KP3)]           = GLFW_KEY_KP_3;
    map[static_cast<size_t>(Key::KP4)]           = GLFW_KEY_KP_4;
    map[static_cast<size_t>(Key::KP5)]           = GLFW_KEY_KP_5;
    map[static_cast<size_t>(Key::KP6)]           = GLFW_KEY_KP_6;
    map[static_cast<size_t>(Key::KP7)]           = GLFW_KEY_KP_7;
    map[static_cast<size_t>(Key::KP8)]           = GLFW_KEY_KP_8;
    map[static_cast<size_t>(Key::KP9)]           = GLFW_KEY_KP_9;
    map[static_cast<size_t>(Key::KPDecimal)]     = GLFW_KEY_KP_DECIMAL;
    map[static_cast<size_t>(Key::KPDivide)]      = GLFW_KEY_KP_DIVIDE;
    map[static_cast<size_t>(Key::KPMultiply)]    = GLFW_KEY_KP_MULTIPLY;
    map[static_cast<size_t>(Key::KPSubtract)]    = GLFW_KEY_KP_SUBTRACT;
    map[static_cast<size_t>(Key::KPAdd)]         = GLFW_KEY_KP_ADD;
    map[static_cast<size_t>(Key::KPEnter)]       = GLFW_KEY_KP_ENTER;
    map[static_cast<size_t>(Key::KPEqual)]       = GLFW_KEY_KP_EQUAL;

    // Modifiers
    map[static_cast<size_t>(Key::LeftShift)]     = GLFW_KEY_LEFT_SHIFT;
    map[static_cast<size_t>(Key::LeftControl)]   = GLFW_KEY_LEFT_CONTROL;
    map[static_cast<size_t>(Key::LeftAlt)]       = GLFW_KEY_LEFT_ALT;
    map[static_cast<size_t>(Key::LeftSuper)]     = GLFW_KEY_LEFT_SUPER;
    map[static_cast<size_t>(Key::RightShift)]    = GLFW_KEY_RIGHT_SHIFT;
    map[static_cast<size_t>(Key::RightControl)]  = GLFW_KEY_RIGHT_CONTROL;
    map[static_cast<size_t>(Key::RightAlt)]      = GLFW_KEY_RIGHT_ALT;
    map[static_cast<size_t>(Key::RightSuper)]    = GLFW_KEY_RIGHT_SUPER;
    map[static_cast<size_t>(Key::Menu)]          = GLFW_KEY_MENU;

    return map;
  }();

  auto index = static_cast<size_t>(key);
  return (index < key_map.size()) ? key_map[index] : GLFW_KEY_UNKNOWN;
}

Key GlfwDriver::glfw_to_key(int glfw_key)
{
    static const auto reverse_map = []() {
        std::array<Key, GLFW_KEY_LAST + 1> map;
        map.fill(Key::Unknown);

        // Alphanumeric
        map[GLFW_KEY_SPACE]         = Key::Space;
        map[GLFW_KEY_APOSTROPHE]    = Key::Apostrophe;
        map[GLFW_KEY_COMMA]         = Key::Comma;
        map[GLFW_KEY_MINUS]         = Key::Minus;
        map[GLFW_KEY_PERIOD]        = Key::Period;
        map[GLFW_KEY_SLASH]         = Key::Slash;
        map[GLFW_KEY_0]             = Key::D0;
        map[GLFW_KEY_1]             = Key::D1;
        map[GLFW_KEY_2]             = Key::D2;
        map[GLFW_KEY_3]             = Key::D3;
        map[GLFW_KEY_4]             = Key::D4;
        map[GLFW_KEY_5]             = Key::D5;
        map[GLFW_KEY_6]             = Key::D6;
        map[GLFW_KEY_7]             = Key::D7;
        map[GLFW_KEY_8]             = Key::D8;
        map[GLFW_KEY_9]             = Key::D9;
        map[GLFW_KEY_SEMICOLON]     = Key::Semicolon;
        map[GLFW_KEY_EQUAL]         = Key::Equal;
        map[GLFW_KEY_A]             = Key::A;
        map[GLFW_KEY_B]             = Key::B;
        map[GLFW_KEY_C]             = Key::C;
        map[GLFW_KEY_D]             = Key::D;
        map[GLFW_KEY_E]             = Key::E;
        map[GLFW_KEY_F]             = Key::F;
        map[GLFW_KEY_G]             = Key::G;
        map[GLFW_KEY_H]             = Key::H;
        map[GLFW_KEY_I]             = Key::I;
        map[GLFW_KEY_J]             = Key::J;
        map[GLFW_KEY_K]             = Key::K;
        map[GLFW_KEY_L]             = Key::L;
        map[GLFW_KEY_M]             = Key::M;
        map[GLFW_KEY_N]             = Key::N;
        map[GLFW_KEY_O]             = Key::O;
        map[GLFW_KEY_P]             = Key::P;
        map[GLFW_KEY_Q]             = Key::Q;
        map[GLFW_KEY_R]             = Key::R;
        map[GLFW_KEY_S]             = Key::S;
        map[GLFW_KEY_T]             = Key::T;
        map[GLFW_KEY_U]             = Key::U;
        map[GLFW_KEY_V]             = Key::V;
        map[GLFW_KEY_W]             = Key::W;
        map[GLFW_KEY_X]             = Key::X;
        map[GLFW_KEY_Y]             = Key::Y;
        map[GLFW_KEY_Z]             = Key::Z;
        map[GLFW_KEY_LEFT_BRACKET]  = Key::LeftBracket;
        map[GLFW_KEY_BACKSLASH]     = Key::Backslash;
        map[GLFW_KEY_RIGHT_BRACKET] = Key::RightBracket;
        map[GLFW_KEY_GRAVE_ACCENT]  = Key::GraveAccent;
        map[GLFW_KEY_WORLD_1]       = Key::World1;
        map[GLFW_KEY_WORLD_2]       = Key::World2;

        // Function keys
        map[GLFW_KEY_ESCAPE]        = Key::Escape;
        map[GLFW_KEY_ENTER]         = Key::Enter;
        map[GLFW_KEY_TAB]           = Key::Tab;
        map[GLFW_KEY_BACKSPACE]     = Key::Backspace;
        map[GLFW_KEY_INSERT]        = Key::Insert;
        map[GLFW_KEY_DELETE]        = Key::Delete;
        map[GLFW_KEY_RIGHT]         = Key::Right;
        map[GLFW_KEY_LEFT]          = Key::Left;
        map[GLFW_KEY_DOWN]          = Key::Down;
        map[GLFW_KEY_UP]            = Key::Up;
        map[GLFW_KEY_PAGE_UP]       = Key::PageUp;
        map[GLFW_KEY_PAGE_DOWN]     = Key::PageDown;
        map[GLFW_KEY_HOME]          = Key::Home;
        map[GLFW_KEY_END]           = Key::End;
        map[GLFW_KEY_CAPS_LOCK]     = Key::CapsLock;
        map[GLFW_KEY_SCROLL_LOCK]   = Key::ScrollLock;
        map[GLFW_KEY_NUM_LOCK]      = Key::NumLock;
        map[GLFW_KEY_PRINT_SCREEN]  = Key::PrintScreen;
        map[GLFW_KEY_PAUSE]         = Key::Pause;
        map[GLFW_KEY_F1]            = Key::F1;
        map[GLFW_KEY_F2]            = Key::F2;
        map[GLFW_KEY_F3]            = Key::F3;
        map[GLFW_KEY_F4]            = Key::F4;
        map[GLFW_KEY_F5]            = Key::F5;
        map[GLFW_KEY_F6]            = Key::F6;
        map[GLFW_KEY_F7]            = Key::F7;
        map[GLFW_KEY_F8]            = Key::F8;
        map[GLFW_KEY_F9]            = Key::F9;
        map[GLFW_KEY_F10]           = Key::F10;
        map[GLFW_KEY_F11]           = Key::F11;
        map[GLFW_KEY_F12]           = Key::F12;
        map[GLFW_KEY_F13]           = Key::F13;
        map[GLFW_KEY_F14]           = Key::F14;
        map[GLFW_KEY_F15]           = Key::F15;
        map[GLFW_KEY_F16]           = Key::F16;
        map[GLFW_KEY_F17]           = Key::F17;
        map[GLFW_KEY_F18]           = Key::F18;
        map[GLFW_KEY_F19]           = Key::F19;
        map[GLFW_KEY_F20]           = Key::F20;
        map[GLFW_KEY_F21]           = Key::F21;
        map[GLFW_KEY_F22]           = Key::F22;
        map[GLFW_KEY_F23]           = Key::F23;
        map[GLFW_KEY_F24]           = Key::F24;
        map[GLFW_KEY_F25]           = Key::F25;

        // Keypad
        map[GLFW_KEY_KP_0]          = Key::KP0;
        map[GLFW_KEY_KP_1]          = Key::KP1;
        map[GLFW_KEY_KP_2]          = Key::KP2;
        map[GLFW_KEY_KP_3]          = Key::KP3;
        map[GLFW_KEY_KP_4]          = Key::KP4;
        map[GLFW_KEY_KP_5]          = Key::KP5;
        map[GLFW_KEY_KP_6]          = Key::KP6;
        map[GLFW_KEY_KP_7]          = Key::KP7;
        map[GLFW_KEY_KP_8]          = Key::KP8;
        map[GLFW_KEY_KP_9]          = Key::KP9;
        map[GLFW_KEY_KP_DECIMAL]    = Key::KPDecimal;
        map[GLFW_KEY_KP_DIVIDE]     = Key::KPDivide;
        map[GLFW_KEY_KP_MULTIPLY]   = Key::KPMultiply;
        map[GLFW_KEY_KP_SUBTRACT]   = Key::KPSubtract;
        map[GLFW_KEY_KP_ADD]        = Key::KPAdd;
        map[GLFW_KEY_KP_ENTER]      = Key::KPEnter;
        map[GLFW_KEY_KP_EQUAL]      = Key::KPEqual;

        // Modifiers
        map[GLFW_KEY_LEFT_SHIFT]    = Key::LeftShift;
        map[GLFW_KEY_LEFT_CONTROL]  = Key::LeftControl;
        map[GLFW_KEY_LEFT_ALT]      = Key::LeftAlt;
        map[GLFW_KEY_LEFT_SUPER]    = Key::LeftSuper;
        map[GLFW_KEY_RIGHT_SHIFT]   = Key::RightShift;
        map[GLFW_KEY_RIGHT_CONTROL] = Key::RightControl;
        map[GLFW_KEY_RIGHT_ALT]     = Key::RightAlt;
        map[GLFW_KEY_RIGHT_SUPER]   = Key::RightSuper;
        map[GLFW_KEY_MENU]          = Key::Menu;

        return map;
    }();

    if (glfw_key >= 0 && glfw_key <= GLFW_KEY_LAST) {
        return reverse_map[static_cast<size_t>(glfw_key)];
    }
    return Key::Unknown;
}

void GlfwDriver::use_core_profile()
{
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  INFO("Glfw: set OpenGL profile to core");
  return;
}

void GlfwDriver::set_hints_apple()
{
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  return;
}
