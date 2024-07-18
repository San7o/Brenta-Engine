#include <iostream>
#include "screen.h"

using namespace ECS;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const bool isMouseCaptured = false;

int main() {

    Screen::Init(SCR_WIDTH, SCR_HEIGHT, isMouseCaptured);

    while(!Screen::isWindowClosed()) {

        if (Screen::isKeyPressed(GLFW_KEY_ESCAPE))
            Screen::SetClose();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }

    Screen::Terminate();
    return 0;
}
