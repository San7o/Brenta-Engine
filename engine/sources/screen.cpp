#include "screen.h"

#include <cstdio>
#include "engine_logger.h"

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
}

void Screen::SetMouseCapture(bool isCaptured)
{
    if (isCaptured)
        glfwSetInputMode(Screen::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(Screen::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Screen::SetClose()
{
    glfwSetWindowShouldClose(Screen::window, GLFW_TRUE);
}

void Screen::Terminate()
{
    glfwTerminate();

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

#ifdef __APPLE__
    SetHintsApple();

#endif

    CreateWindow(SCR_WIDTH, SCR_HEIGHT, title);
    MakeContextCurrent();
    SetMouseCapture(isMouseCaptured);
}

void Screen::SetContextVersion(int major, int minor)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

    Logger::Log(Types::LogLevel::INFO, "Set context to OpenGL version: " +
                std::to_string(major) + "." + std::to_string(minor));
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

