#include <cstdio>

#include "screen.h"
#include "engine_input.h"
#include "engine_logger.h"
#include "engine_audio.h"

using namespace ECS;

GLFWwindow* Screen::window;
int Screen::WIDTH;
int Screen::HEIGHT;

bool Screen::isWindowClosed()
{
    return glfwWindowShouldClose(Screen::window);
}

bool Screen::isKeyPressed(int key)
{
    return glfwGetKey(Screen::window, key) == GLFW_PRESS;
}

float Screen::GetTime()
{
    return glfwGetTime();
}

GLFWwindow* Screen::GetWindow()
{
    return Screen::window;
}

GLFWglproc Screen::GetProcAddress()
{
    return reinterpret_cast<void (*)()>(glfwGetProcAddress);
}

int Screen::GetWidth()
{
    return Screen::WIDTH;
}

int Screen::GetHeight()
{
    return Screen::HEIGHT;
}

void Screen::SetMouseCallback(GLFWcursorposfun callback)
{   
    glfwSetCursorPosCallback(Screen::window, callback);
}

void Screen::SetSizeCallback(GLFWframebuffersizefun callback)
{
    glfwSetFramebufferSizeCallback(Screen::window, callback);

    Logger::Log(Types::LogLevel::INFO, "Set framebuffer size callback");
}

void Screen::SetMouseCapture(bool isCaptured)
{
    if (isCaptured) {
        glfwSetInputMode(Screen::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        Logger::Log(Types::LogLevel::INFO, "Mouse captured");
    } else {
        glfwSetInputMode(Screen::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        Logger::Log(Types::LogLevel::INFO, "Mouse not captured");
    }
}

void Screen::SetClose()
{
    glfwSetWindowShouldClose(Screen::window, GLFW_TRUE);
}

void Screen::Terminate()
{
    glfwTerminate();
    Audio::Destroy();
    Logger::Log(Types::LogLevel::INFO, "Screen terminated");
}

void Screen::SwapBuffers()
{
    glfwSwapBuffers(Screen::window);
}

void Screen::PollEvents()
{
    glfwPollEvents();
}

void Screen::Init(int SCR_WIDTH, int SCR_HEIGHT,
                  bool isMouseCaptured, const char* title)
{
    Screen::WIDTH = SCR_WIDTH;
    Screen::HEIGHT = SCR_HEIGHT;

    if (glfwInit() == GLFW_FALSE)
    {
        Logger::Log(Types::LogLevel::ERROR,
                        "Failed to initialize GLFW on init");
    }

    SetContextVersion(3, 3); /* OpenGL 3.3 */
    UseCoreProfile();

    glfwWindowHint(GLFW_SAMPLES, 4); /* MSAA */
    Logger::Log(Types::LogLevel::INFO, "Enabled MSAA");

    glfwSwapInterval(0); /* Disable VSync */
    Logger::Log(Types::LogLevel::INFO, "Disabled VSync");

#ifdef __APPLE__
    SetHintsApple();
#endif

    CreateWindow(SCR_WIDTH, SCR_HEIGHT, title);
    MakeContextCurrent();
    SetMouseCapture(isMouseCaptured);

    /* Set the callback for resizing the window */
    Screen::SetSizeCallback(Framebuffer_size_callback);

    Input::Init();
    Audio::Init();
}

void Screen::SetContextVersion(int major, int minor)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

    Logger::Log(Types::LogLevel::INFO, "Set context to OpenGL version: " +
                std::to_string(major) + "." + std::to_string(minor));
}

void Screen::SetKeyCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(Screen::window, callback);
}

void Screen::UseCoreProfile()
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Logger::Log(Types::LogLevel::INFO, "Set OpenGL profile to core");
}

void Screen::SetHintsApple()
{
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Screen::CreateWindow(int SCR_WIDTH, int SCR_HEIGHT, const char* title)
{
    Screen::window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if (Screen::window == NULL)
    {
        Logger::Log(Types::LogLevel::ERROR, "Failed to create GLFW window");
        Terminate();
    }
}

void Screen::MakeContextCurrent()
{
    glfwMakeContextCurrent(Screen::window);
}


void Screen::Framebuffer_size_callback(GLFWwindow* window, int width,
                               int height)
{
    glViewport(0, 0, width, height);
    Screen::WIDTH = width;
    Screen::HEIGHT = height;

    Logger::Log(Types::LogLevel::INFO, "Set viewport: " +
                std::to_string(width) + "x" + std::to_string(height));
}
