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

#pragma once

#include "components/physics_component.hpp"
#include "components/transform_component.hpp"
#include "engine.hpp"
#include "systems/physics_system.hpp"
#include "viotecs/viotecs.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace viotecs;
using namespace viotecs::types;

struct PhysicsSystem : system<PhysicsComponent, TransformComponent>
{
    void run(std::vector<entity_t> matches) const override
    {
        if (matches.empty())
            return;

        for (auto match : matches)
        {
            auto physics_component =
                world::entity_to_component<PhysicsComponent>(match);

            auto transform_component =
                world::entity_to_component<TransformComponent>(match);

            if (physics_component->acceleration != glm::vec3(0.0f))
            {
                physics_component->velocity +=
                    physics_component->acceleration * time::get_delta_time();
            }
            if (physics_component->velocity != glm::vec3(0.0f))
            {
                transform_component->position +=
                    physics_component->velocity * time::get_delta_time();
            }
        }
    }
};
