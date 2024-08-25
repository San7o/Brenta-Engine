/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/**
 * Examples of various systems that can be used in an ECS system.
 */

#pragma once

#include "ecs.hpp"
#include "engine.hpp"
#include "systems/fps_system.hpp"

#include <vector>

using namespace Brenta::ECS;
using namespace Brenta::ECS::Types;

struct FPSSystem : System<None>
{
    void run(std::vector<Entity> _) const override
    {
        Text::RenderText("FPS: " + std::to_string(Time::GetFPS()), 25.0f, 25.0f,
                         0.35f, glm::vec3(1.0f, 0.9f, 0.0f));
    }
};

struct PhysicsSystem : System<PhysicsComponent, TransformComponent>
{
    void run(std::vector<Entity> matches) const override
    {
        if (matches.empty())
            return;

        for (auto match : matches)
        {
            auto physics_component =
                World::EntityToComponent<PhysicsComponent>(match);

            auto transform_component =
                World::EntityToComponent<TransformComponent>(match);

            if (physics_component->acceleration != glm::vec3(0.0f))
            {
                physics_component->velocity +=
                    physics_component->acceleration * Time::GetDeltaTime();
            }
            if (physics_component->velocity != glm::vec3(0.0f))
            {
                transform_component->position +=
                    physics_component->velocity * Time::GetDeltaTime();
            }
        }
    }
};
