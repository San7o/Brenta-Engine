#include <iostream>
#include <filesystem>

#include "engine.h"

#include "model_component.h"
#include "player_component.h"
#include "renderer_system.h"

using namespace ECS;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main() {

    /* Set Log level, default = WARNING */
    Logger::SetLogLevel(Types::LogLevel::INFO);

    /* Initialize the screen */
    Screen::Init(SCR_WIDTH, SCR_HEIGHT);

    /* Load OpenGL */
    GL::LoadOpenGL();

    /* Initialize the world */
    World::Init();

    InitPlayer();
    InitRenderer();
    // ...

    while(!Screen::isWindowClosed()) {

        /* Input */
        if (Screen::isKeyPressed(GLFW_KEY_ESCAPE))
            Screen::SetClose();

        /* Clear */
        GL::SetColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::Clear();

        World::Tick();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }
    
    World::Delete();
    Screen::Terminate();
    return 0;
}
