#include "engine.h"

#include "entities/player_entity.h"
#include "systems/renderer_system.h"
#include "callbacks/toggle_wireframe_callback.h"
#include "callbacks/close_window_callback.h"
#include "resources/wireframe_resource.h"
#include "systems/fps_system.h"

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

    InitPlayerEntity();
    InitRendererSystem();
    InitToggleWireframeCallback();
    InitCloseWindowCallback();
    InitWireframeResource();
    InitFpsSystem();

    while(!Screen::isWindowClosed()) {

        GL::SetColor(0.1f, 0.1f, 0.1f, 1.0f);
        GL::Clear();

        World::Tick();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }
    
    World::Delete();
    Screen::Terminate();
    return 0;
}


