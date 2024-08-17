#include "engine.h"
#include "game_ecs.h"

#include <filesystem>
#include <bitset>

using namespace ECS;

// Default resolution
const int SCR_WIDTH = 1280; 
const int SCR_HEIGHT = 720;

int main() {

    Logger::Init();                              /* defauls to ./logs/log.txt */
    Logger::SetLogLevel(Types::LogLevel::DEBUG); /* default = WARNING */

    Screen::Init(SCR_WIDTH, SCR_HEIGHT);
    GL::LoadOpenGL();
    World::Init();

    InitCloseWindowCallback();
    InitDashCallback();

    InitPlayerEntity();

    InitPlayerMovementSystem();
    InitPhysicsSystem();
    InitRenderParticleSystem();
    InitRendererSystem();

    /* Camera */
    Camera::eulerAngles = {-90.0f, 0.0f, 0.0f};
    Camera::Position = {0.0f, 0.0f, 20.0f};
    Camera::updateCameraEuler();
    
    while(!Screen::isWindowClosed()) {

        GL::SetColor(0.2f, 0.2f, 0.207f, 1.0f);
        GL::Clear();

        World::Tick();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }
    
    World::Delete();
    Screen::Terminate();
    Logger::Close();
    return 0;
}


