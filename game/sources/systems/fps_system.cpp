#include "systems/fps_system.h"

#include "engine.h"

using namespace ECS;
using namespace ECS::Types;

void InitFpsSystem() {

    auto show_fps = std::make_shared<System>("ShowFPSSystem", []() {
        Text::RenderText(
                "FPS: " + std::to_string(Time::GetFPS()),
                25.0f, 25.0f, 0.35f,
                glm::vec3(1.0f, 0.9f, 0.0f));
    });

    World::AddSystem(show_fps);
    
}
