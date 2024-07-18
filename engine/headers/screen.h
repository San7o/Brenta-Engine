#ifndef SCREEN_H
#define SCREEN_H

#include <GLFW/glfw3.h>      /* OpenGL windowing library */

namespace ECS {

class Screen {
public:
    static GLFWwindow* window;

    Screen() = delete;
    static void Init(int SCR_WIDTH, int SCR_HEIGHT, bool isMouseCaptured);

    static bool isWindowClosed();
    static bool isKeyPressed(int key);
    static float GetTime();
    static GLFWwindow* GetWindow();
    static GLFWglproc GetProcAddress();

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
    static void CreateWindow(int SCR_WIDTH, int SCR_HEIGHT);
    static void MakeContextCurrent();
};

} // namespace ECS

#endif
