#include <iostream>
#include "gl_helper.h"
#include "screen.h"

using namespace ECS;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const bool isMouseCaptured = false;

int main() {

    Screen::Init(SCR_WIDTH, SCR_HEIGHT);

    GL::LoadOpenGL();

    while(!Screen::isWindowClosed()) {

        /* Input */
        if (Screen::isKeyPressed(GLFW_KEY_ESCAPE))
            Screen::SetClose();

        /* Clear */
        GL::SetColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::Clear();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }

    Screen::Terminate();
    return 0;
}
