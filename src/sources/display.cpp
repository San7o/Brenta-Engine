#include <GLFW/glfw3.h>      /* OpenGL windowing library */
#include <cstdio>

#include "display.h"


Display::Display() {}

Display::Display(int SCR_WIDTH, int SCR_HEIGHT)
{
    init();
    SetContextVersion(3, 3); /* OpenGL 3.3 */
    UseCoreProfile();
    glfwWindowHint(GLFW_SAMPLES, 4); /* MSAA */
#ifdef __APPLE__
    SetHintsApple();
#endif
    CreateWindow(SCR_WIDTH, SCR_HEIGHT);
    MakeContextCurrent();
    SetMouseCapture(true);
}

bool Display::isWindowClosed()
{
    return glfwWindowShouldClose(window);
}

bool Display::isKeyPressed(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

float Display::GetTime()
{
    return glfwGetTime();
}

GLFWwindow* Display::GetWindow()
{
    return window;
}

GLFWglproc Display::GetProcAddress()
{
    return reinterpret_cast<void (*)()>(glfwGetProcAddress);
}

void Display::SetMouseCallback(GLFWcursorposfun callback)
{   
    glfwSetCursorPosCallback(window, callback);
}

void Display::SetSizeCallback(GLFWframebuffersizefun callback)
{
    glfwSetFramebufferSizeCallback(window, callback);
}

void Display::SetMouseCapture(bool isCaptured)
{
    if (isCaptured)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Display::SetClose()
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Display::Terminate()
{
    glfwTerminate();
}

void Display::SwapBuffers()
{
    glfwSwapBuffers(window);
}

void Display::PollEvents()
{
    glfwPollEvents();
}

void Display::init()
{
    if (glfwInit() == GLFW_FALSE)
    {
        fprintf(stderr, "Failed to initialize GLFW on init\n");
    }
}

void Display::SetContextVersion(int major, int minor)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

void Display::UseCoreProfile()
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Display::SetHintsApple()
{
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Display::CreateWindow(int SCR_WIDTH, int SCR_HEIGHT)
{
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        Terminate();
    }
}

void Display::MakeContextCurrent()
{
    glfwMakeContextCurrent(window);
}

