#include <GLFW/glfw3.h>      /* OpenGL windowing library */
#include <cstdio>

#include "screen.h"

using namespace ECS;

GLFWwindow* Screen::window;

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
}

void Screen::SwapBuffers()
{
    glfwSwapBuffers(Screen::window);
}

void Screen::PollEvents()
{
    glfwPollEvents();
}

void Screen::Init(int SCR_WIDTH, int SCR_HEIGHT, bool isMouseCaptured)
{
    if (glfwInit() == GLFW_FALSE)
    {
        fprintf(stderr, "Failed to initialize GLFW on init\n");
    }

    SetContextVersion(3, 3); /* OpenGL 3.3 */
    UseCoreProfile();
    glfwWindowHint(GLFW_SAMPLES, 4); /* MSAA */
    /* Debug context. REMOVE IN PRODUCTION */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#ifdef __APPLE__
    SetHintsApple();
#endif
    CreateWindow(SCR_WIDTH, SCR_HEIGHT);
    MakeContextCurrent();
    SetMouseCapture(isMouseCaptured);
}

void Screen::SetContextVersion(int major, int minor)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

void Screen::UseCoreProfile()
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Screen::SetHintsApple()
{
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Screen::CreateWindow(int SCR_WIDTH, int SCR_HEIGHT)
{
    Screen::window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (Screen::window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        Terminate();
    }
}

void Screen::MakeContextCurrent()
{
    glfwMakeContextCurrent(Screen::window);
}

