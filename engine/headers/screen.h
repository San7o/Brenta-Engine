#pragma once

#ifndef SCREEN_H
#define SCREEN_H

#ifndef __glfw3_h_
#include <GLFW/glfw3.h>      /* OpenGL windowing library */
#endif

namespace ECS {

class Screen {
public:
    static int WIDTH;
    static int HEIGHT;
    static GLFWwindow* window;

    Screen() = delete;
    static void Init(int SCR_WIDTH, int SCR_HEIGHT,
                     bool isMouseCaptured = false,
                     const char* title = "OpenGL");

    static bool isWindowClosed();
    static bool isKeyPressed(int key);
    static float GetTime();
    static GLFWwindow* GetWindow();
    static GLFWglproc GetProcAddress();
    static int GetWidth();
    static int GetHeight();

    static void SetMouseCallback(GLFWcursorposfun callback);
    static void SetSizeCallback(GLFWframebuffersizefun callback);
    static void SetMouseCapture(bool isCaptured);
    static void SetClose();

    /* Swap the front and back buffers to avoid flickering */
    static void SwapBuffers();
    /* Process all pending events */
    static void PollEvents();
    static void Terminate();

private:
    static void SetContextVersion(int major, int minor);
    static void UseCoreProfile();
    static void SetHintsApple();
    static void CreateWindow(int SCR_WIDTH, int SCR_HEIGHT, const char* title);
    static void MakeContextCurrent();
};

} // namespace ECS

#endif
