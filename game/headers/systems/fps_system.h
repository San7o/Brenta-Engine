#pragma once

#include "systems/fps_system.h"
#include "engine.h"
#include "ecs.h"

#include <vector>

using namespace ECS;
using namespace ECS::Types;

struct FPSSystem : System<None> {

    void run(std::vector<Entity> _) const override {
        Text::RenderText(
                "FPS: " + std::to_string(Time::GetFPS()),
                25.0f, 25.0f, 0.35f,
                glm::vec3(1.0f, 0.9f, 0.0f));
    }
};
