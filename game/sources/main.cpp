#include "engine.h"
#include "ecs.h"
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

    InitPlayerEntity();
    //InitCubeEntity();
    InitFloorEntity();
    InitDirectionalLightEntity();
    InitPointLightEntity();
    InitSphereEntity();
    InitRobotEntity();

    InitDirectionalLightSystem();
    InitPointLightsSystem();
    InitRendererSystem();
    //InitFpsSystem();
    InitDebugTextSystem();
    InitPhysicsSystem();
    InitCollisionsSystem();

    InitToggleWireframeCallback();
    InitCloseWindowCallback();
    InitCameraMouseCallback();
    InitPlayGuitarCallback();
    
    InitWireframeResource();

    Audio::LoadAudio("guitar",
             std::filesystem::absolute("assets/audio/guitar.wav"));

    ParticleEmitter emitter(
        glm::vec3(0.0f, 0.0f, 5.0f),    /* starting position */
        glm::vec3(0.0f, 5.0f, 0.0f),    /* starting velocity */
        glm::vec3(10.0f, 10.0f, 10.0f), /* starting spread */
        0.5f,                           /* starting time to live */
        1000,                           /* number of particles */
        0.01f,                          /* spawn rate */
        1.0f,                           /* particle scale */
        std::filesystem::absolute(
            "assets/textures/particle_atlas.png"
        ).string(),                  /* atlas texture */ 
        8,                           /* atlas width */
        8,                           /* atlas height */
        45                           /* atlas index */
    );

    Time::Update(Screen::GetTime());
    while(!Screen::isWindowClosed()) {

        GL::SetColor(0.2f, 0.2f, 0.207f, 1.0f);
        GL::Clear();

        emitter.updateParticles(Time::GetDeltaTime());
        emitter.renderParticles();

        World::Tick();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }
    
    World::Delete();
    Screen::Terminate();
    Logger::Close();
    return 0;
}


