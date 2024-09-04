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

#include "entities/sphere_entity.hpp"

#include "components/model_component.hpp"
#include "components/physics_component.hpp"
#include "components/sphere_collider_component.hpp"
#include "components/transform_component.hpp"
#include "ecs.hpp"
#include "engine.hpp"

#include <filesystem>

using namespace brenta::ecs;
using namespace brenta::ecs::types;
using namespace brenta;

void init_sphere_entity()
{
    /* ************************************************
     *
     *    Sphere 1
     *
     * ************************************************ */

    /* Create the sphere entity */
    auto sphere_entity1 = world::new_entity();

    /* Add the transform component */
    auto transform_component1 =
        TransformComponent(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0.0f), 1.0f);
    world::add_component<TransformComponent>(sphere_entity1,
                                            transform_component1);

    /* Add the physics component */
    auto physics_component1 =
        PhysicsComponent(1.0f,                        // mass
                         1.0f,                        // density
                         glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                         glm::vec3(-1.f, 0.0f, 0.0f), // acceleration
                         true                         // isElastic
        );
    world::add_component<PhysicsComponent>(sphere_entity1, physics_component1);

    /* Add the sphere collider component */
    auto sphere_collider_component1 = SphereColliderComponent(1.0f);
    world::add_component<SphereColliderComponent>(sphere_entity1,
                                                 sphere_collider_component1);

    /* Load the shader */
    if (shader::get_id("default_shader") == 0)
    {
        shader::create("default_shader", GL_VERTEX_SHADER,
                    std::filesystem::absolute("game/shaders/shader.vs"),
                    GL_FRAGMENT_SHADER,
                    std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    model m1(std::filesystem::absolute("assets/models/sphere/sphere.obj"));

    /* Add the model component */
    auto model_component1 = ModelComponent(m1, 32.0f, "default_shader");
    world::add_component<ModelComponent>(sphere_entity1,
                                        std::move(model_component1));

    /* ************************************************
     *
     *    Sphere 2
     *
     * ************************************************ */

    /* Create the sphere entity */
    auto sphere_entity2 = world::new_entity();

    /* Add the transform component */
    auto transform_component2 =
        TransformComponent(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(0.0f), 1.0f);
    world::add_component<TransformComponent>(sphere_entity2,
                                            transform_component2);

    /* Add the physics component */
    auto physics_component2 =
        PhysicsComponent(1.0f,                        // mass
                         1.0f,                        // density
                         glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                         glm::vec3(1.0f, 0.0f, 0.0f), // acceleration
                         true                         // isElastic
        );
    world::add_component<PhysicsComponent>(sphere_entity2, physics_component2);

    /* Add the sphere collider component */
    auto sphere_collider_component2 = SphereColliderComponent(1.0f);
    world::add_component<SphereColliderComponent>(sphere_entity2,
                                                 sphere_collider_component2);

    /* Load the shader */
    if (shader::get_id("default_shader") == 0)
    {
        shader::create("default_shader", GL_VERTEX_SHADER,
                    std::filesystem::absolute("game/shaders/shader.vs"),
                    GL_FRAGMENT_SHADER,
                    std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    model m2(std::filesystem::absolute("assets/models/sphere/sphere.obj"));

    /* Add the model component */
    auto model_component2 = ModelComponent(m2, 32.0f, "default_shader");
    world::add_component<ModelComponent>(sphere_entity2,
                                        std::move(model_component2));
}
