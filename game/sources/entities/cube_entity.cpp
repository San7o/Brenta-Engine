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

#include "entities/cube_entity.hpp"

#include "components/model_component.hpp"
#include "components/transform_component.hpp"
#include "ecs.hpp"
#include "engine.hpp"

#include <filesystem>

using namespace brenta::ecs;
using namespace brenta::ecs::types;
using namespace brenta;

void init_cube_entity()
{
    /* Create the cube entity */
    auto cube_entity = world::new_entity();

    /* Add the transform component */
    auto transform_component =
        TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), 1.0f);
    world::add_component<TransformComponent>(cube_entity, transform_component);

    /* Load the shader */
    if (shader::get_id("cube_shader") == 0)
    {
        shader::create("cube_shader", GL_VERTEX_SHADER,
                       std::filesystem::absolute("game/shaders/shader.vs"),
                       GL_FRAGMENT_SHADER,
                       std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    model m(
        std::filesystem::absolute("assets/models/simple_cube/simple_cube.obj"));

    /* Add the model component */
    auto model_component = ModelComponent(m, 32.0f, "cube_shader");
    world::add_component<ModelComponent>(cube_entity,
                                         std::move(model_component));
}
