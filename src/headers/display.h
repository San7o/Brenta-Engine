#ifndef DISPLAY_H
#define DISPLAY_H

#include <GLFW/glfw3.h>      /* OpenGL windowing library */

class Display
{
public:
    GLFWwindow* window;
 
    /* Empty constructor */
    Display();
    /* Constructor with parameters */
    Display(int SCR_WIDTH, int SCR_HEIGHT);

    bool isWindowClosed();
    bool isKeyPressed(int key);
    float GetTime();
    GLFWwindow* GetWindow();
    GLFWglproc GetProcAddress();

    void SetMouseCallback(GLFWcursorposfun callback);
    void SetSizeCallback(GLFWframebuffersizefun callback);
    void SetMouseCapture(bool isCaptured);
    void SetClose();

    /* Swap the front and back buffers to avoid flickering */
    void SwapBuffers();
    /* Process all pending events */
    void PollEvents();
    void Terminate();

private:
    void init();
    void SetContextVersion(int major, int minor);
    void UseCoreProfile();
    void SetHintsApple();
    void CreateWindow(int SCR_WIDTH, int SCR_HEIGHT);
    void MakeContextCurrent();
};
#endif
