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

#include "entities/point_light_entity.hpp"

#include "components/model_component.hpp"
#include "components/point_light_component.hpp"
#include "components/transform_component.hpp"
#include "ecs.hpp"
#include "engine.hpp"

#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace Brenta::ECS;
using namespace Brenta;

void InitPointLightEntity()
{
    /* Create the light entity */
    auto light_entity = World::NewEntity();

    /* Add the light component */
    auto light_component = PointLightComponent(
        glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.0032f, 1.0f,
        std::vector<Brenta::Types::ShaderName>{"default_shader"});
    World::AddComponent<PointLightComponent>(light_entity, light_component);

    /* Add a mesh */
    /*
    if (Shader::GetId("default_shader") == 0) {
        Shader::NewShader("default_shader",
                          std::filesystem::absolute("game/shaders/shader.vs"),
                          std::filesystem::absolute("game/shaders/shader.fs"));
    }
    Model
    model(std::filesystem::absolute("assets/models/simple_cube/simple_cube.obj"));
    auto model_component = ModelComponent(model, 32.0f, "default_shader");
    World::AddComponent<ModelComponent>(light_entity, model_component);
    */

    /* Add the transform component */
    auto transform_component =
        TransformComponent(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f), 0.5f);
    World::AddComponent<TransformComponent>(light_entity, transform_component);
}
