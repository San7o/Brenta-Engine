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

#include "entities/directional_light_entity.hpp"

#include "components/directional_light_component.hpp"
#include "components/model_component.hpp"
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

void InitDirectionalLightEntity()
{
    /* Create the light entity */
    auto light_entity = World::NewEntity();

    /* Add the light component */
    auto light_component = DirectionalLightComponent(
        glm::vec3(0.2f, -1.0f, -0.3f), // direction
        glm::vec3(0.7f, 0.7f, 0.7f),   // ambient
        glm::vec3(0.5f, 0.5f, 0.5f),   // diffuse
        glm::vec3(1.0f, 1.0f, 1.0f),   // specular
        1.0f,                          // intensity
        std::vector<Brenta::Types::ShaderName>{"default_shader"});
    World::AddComponent<DirectionalLightComponent>(light_entity,
                                                   std::move(light_component));
}
