#include "engine.hpp"

#ifdef USE_ECS
#include "ecs.hpp"
#endif

using namespace brenta;

#ifdef USE_ECS
using namespace brenta::ecs;
#endif

engine::engine(bool uses_screen, bool uses_audio, bool uses_input,
               bool uses_logger, bool uses_text, int screen_width,
               int screen_height, bool screen_is_mouse_captured,
               bool screen_msaa, bool screen_vsync, const char *screen_title,
               types::log_level log_level, std::string log_file,
               std::string text_font, int text_size, bool gl_blending,
               bool gl_cull_face, bool gl_multisample, bool gl_depth_test)
{
    if (uses_screen)
    {
        screen::init(screen_width, screen_height, screen_is_mouse_captured,
                     screen_title, screen_msaa, screen_vsync);
        gl::load_opengl(gl_blending, gl_cull_face, gl_multisample,
                       gl_depth_test);
    }

    if (uses_audio)
    {
        audio::init();
    }

    if (uses_input)
    {
        input::init();
    }

    if (uses_logger)
    {
        logger::init();
        logger::set_log_level(log_level);
        logger::set_log_file(log_file);
    }

    if (uses_text)
    {
        text::init();
        text::load(text_font, text_size);
    }
#ifdef USE_ECS
    world::init();
#endif
#ifdef USE_IMGUI
    gui::init();
#endif
}

engine::~engine()
{
#ifdef USE_IMGUI
    gui::destroy();
#endif
#ifdef USE_ECS
    world::destroy();
#endif

    if (uses_logger)
    {
        logger::close();
    }

    if (uses_audio)
    {
        audio::destroy();
    }

    if (uses_screen)
    {
        screen::terminate();
    }
}

engine::builder &engine::builder::use_screen(bool uses_screen)
{
    this->uses_screen = uses_screen;
    return *this;
}

engine::builder &engine::builder::use_audio(bool uses_audio)
{
    this->uses_audio = uses_audio;
    return *this;
}

engine::builder &engine::builder::use_input(bool uses_input)
{
    this->uses_input = uses_input;
    return *this;
}

engine::builder &engine::builder::use_logger(bool uses_logger)
{
    this->uses_logger = uses_logger;
    return *this;
}

engine::builder &engine::builder::use_text(bool uses_text)
{
    this->uses_text = uses_text;
    return *this;
}

engine::builder &engine::builder::set_screen_width(int screen_width)
{
    this->screen_width = screen_width;
    return *this;
}

engine::builder &engine::builder::set_screen_height(int screen_height)
{
    this->screen_height = screen_height;
    return *this;
}

engine::builder &
engine::builder::set_screen_is_mouse_captured(bool screen_is_mouse_captured)
{
    this->screen_is_mouse_captured = screen_is_mouse_captured;
    return *this;
}

engine::builder &engine::builder::set_screen_title(const char *screen_title)
{
    this->screen_title = screen_title;
    return *this;
}

engine::builder &engine::builder::set_screen_msaa(bool screen_msaa)
{
    this->screen_msaa = screen_msaa;
    return *this;
}

engine::builder &engine::builder::set_screen_vsync(bool screen_vsync)
{
    this->screen_vsync = screen_vsync;
    return *this;
}

engine::builder &engine::builder::set_log_level(types::log_level log_level)
{
    this->log_level = log_level;
    return *this;
}

engine::builder &engine::builder::set_log_file(std::string log_file)
{
    this->log_file = log_file;
    return *this;
}

engine::builder &engine::builder::set_text_font(std::string text_font)
{
    this->text_font = text_font;
    return *this;
}

engine::builder &engine::builder::set_text_size(int text_size)
{
    this->text_size = text_size;
    return *this;
}

engine::builder &engine::builder::set_gl_blending(bool gl_blending)
{
    this->gl_blending = gl_blending;
    return *this;
}

engine::builder &engine::builder::set_gl_cull_face(bool gl_cull_face)
{
    this->gl_cull_face = gl_cull_face;
    return *this;
}

engine::builder &engine::builder::set_gl_multisample(bool gl_multisample)
{
    this->gl_multisample = gl_multisample;
    return *this;
}

engine::builder &engine::builder::set_gl_depth_test(bool gl_depth_test)
{
    this->gl_depth_test = gl_depth_test;
    return *this;
}

engine engine::builder::build()
{
    return engine(uses_screen, uses_audio, uses_input, uses_logger, uses_text,
                  screen_width, screen_height, screen_is_mouse_captured,
                  screen_msaa, screen_vsync, screen_title, log_level, log_file,
                  text_font, text_size, gl_blending, gl_cull_face,
                  gl_multisample, gl_depth_test);
}
