#include "engine.h"
#include "ecs.h"
#include "game_ecs.h"

#include <filesystem>
#include <bitset>

using namespace ECS;

// Default resolution
const int SCR_WIDTH = 1280; 
const int SCR_HEIGHT = 720;

REGISTER_SYSTEMS(
    RendererSystem,
    DebugTextSystem,
    PointLightsSystem,
    DirectionalLightSystem,
    PhysicsSystem,
    CollisionsSystem
);

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

    InitToggleWireframeCallback();
    InitCloseWindowCallback();
    InitCameraMouseCallback();
    InitPlayGuitarCallback();

    World::AddResource<WireframeResource>(WireframeResource(false));

    Audio::LoadAudio("guitar",
             std::filesystem::absolute("assets/audio/guitar.wav"));

    /* Nice builder patterns */
    ParticleEmitter emitter = ParticleEmitter::Builder()
            .set_starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
            .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
            .set_starting_spread(glm::vec3(10.0f, 10.0f, 10.0f))
            .set_starting_timeToLive(0.5f)
            .set_num_particles(1000)
            .set_spawn_rate(0.01f)
            .set_scale(1.0f)
            .set_atlas_path(std::filesystem::absolute(
                "assets/textures/particle_atlas.png"
            ).string())
            .set_atlas_width(8)
            .set_atlas_height(8)
            .set_atlas_index(45)
            .build();

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


