#pragma once

#include "ecs.hpp"
#include "engine.hpp"
#include "systems/fps_system.hpp"

#include <vector>

using namespace Brenta::ECS;
using namespace Brenta::ECS::Types;

struct FPSSystem : System<None>
{
    void run (std::vector<Entity> _) const override
    {
        Text::RenderText ("FPS: " + std::to_string (Time::GetFPS ()), 25.0f,
                          25.0f, 0.35f, glm::vec3 (1.0f, 0.9f, 0.0f));
    }
};
