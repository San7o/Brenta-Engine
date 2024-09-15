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
#include "components/sphere_collider_component.hpp"
#include "components/transform_component.hpp"
#include "ecs.hpp"
#include "engine.hpp"
#include "systems/collisions_system.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace brenta::ecs;
using namespace brenta::ecs::types;

glm::vec3 ResolveCollision(glm::vec3 position1, glm::vec3 position2,
                           float radius1, float radius2, float distance);

struct CollisionsSystem : system<SphereColliderComponent, TransformComponent>
{
    void run(std::vector<entity_t> matches) const override
    {
        if (matches.empty())
            return;

        for (long unsigned int i = 0; i < matches.size(); i++)
        {
            for (long unsigned int j = 0; j < matches.size(); j++)
            {
                if (i == j)
                    continue;
                auto sphere_component1 =
                    world::entity_to_component<SphereColliderComponent>(
                        matches[i]);
                auto transform_component1 =
                    world::entity_to_component<TransformComponent>(matches[i]);
                auto sphere_component2 =
                    world::entity_to_component<SphereColliderComponent>(
                        matches[j]);
                auto transform_component2 =
                    world::entity_to_component<TransformComponent>(matches[j]);

                float distance = glm::distance(transform_component1->position,
                                               transform_component2->position);
                if (distance
                    < sphere_component1->radius + sphere_component2->radius)
                {
                    auto physics_component1 =
                        world::entity_to_component<PhysicsComponent>(
                            matches[i]);
                    auto physics_component2 =
                        world::entity_to_component<PhysicsComponent>(
                            matches[j]);
                    if (physics_component1 == nullptr
                        || physics_component2 == nullptr)
                    {
                        continue;
                    }
                    if (physics_component1 != nullptr
                        && physics_component2 == nullptr)
                    {
                        transform_component1->position = ResolveCollision(
                            transform_component1->position,
                            transform_component2->position,
                            sphere_component1->radius,
                            sphere_component2->radius, distance);
                        physics_component1->velocity = glm::vec3(0.0f);
                    }
                    else if (physics_component1 == nullptr
                             && physics_component2 != nullptr)
                    {
                        transform_component2->position = ResolveCollision(
                            transform_component2->position,
                            transform_component1->position,
                            sphere_component2->radius,
                            sphere_component1->radius, distance);
                        physics_component2->velocity = glm::vec3(0.0f);
                    }
                    else
                    {
                        transform_component1->position = ResolveCollision(
                            transform_component1->position,
                            transform_component2->position,
                            sphere_component1->radius,
                            sphere_component2->radius, distance);
                        transform_component2->position = ResolveCollision(
                            transform_component2->position,
                            transform_component1->position,
                            sphere_component2->radius,
                            sphere_component1->radius, distance);
                        physics_component1->velocity =
                            -physics_component1->velocity;
                        physics_component2->velocity =
                            -physics_component2->velocity;
                    }
                }
            }
        }
    }
};

glm::vec3 ResolveCollision(glm::vec3 position1, glm::vec3 position2,
                           float radius1, float radius2, float distance)
{
    return position1
           + glm::normalize(position1 - position2)
                 * (radius1 + radius2 - distance);
}
