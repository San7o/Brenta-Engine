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
        glm::vec3(0.0f, 0.0f, 5.0f), /* starting position */
        glm::vec3(0.0f, 5.0f, 0.0f), /* starting velocity */
        glm::vec3(10.0f, 10.0f, 10.0f), /* starting spread */
        0.5f,                       /* starting time to live */
        1000,                        /* number of particles */
        0.01f,                       /* spawn rate */
        std::filesystem::absolute(
            "assets/textures/particle_atlas.png"
        ).string(),                  /* atlas texture */ 
        8,                           /* atlas width */
        8,                           /* atlas height */
        45                           /* atlas index */
    );

    while(!Screen::isWindowClosed()) {

        GL::SetColor(0.2f, 0.2f, 0.207f, 1.0f);
        GL::Clear();

        emitter.updateParticles(Time::GetDeltaTime());
        emitter.renderParticles();

        // Get feedback
        /*
        glFlush();
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, fbo[current]);
        glm::vec3 feedback[2];
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
        std::cout << feedback[0].x << " "
                  << feedback[0].y << " "
                  << feedback[0].z << ", "
                  << feedback[1].x << " "
                  << feedback[1].y << " "
                  << feedback[1].z << ", ";
        float ttl;
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 2 * sizeof(glm::vec3), sizeof(float), &ttl);
        std::cout << "TTL: " << ttl << std::endl;
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
        */

        World::Tick();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }
    
    World::Delete();
    Screen::Terminate();
    Logger::Close();
    return 0;
}


