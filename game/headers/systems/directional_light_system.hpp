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

#include "components/directional_light_component.hpp"
#include "ecs.hpp"
#include "systems/directional_light_system.hpp"

#include <vector>

using namespace Brenta::ECS;

struct DirectionalLightSystem : System<DirectionalLightComponent>
{
    void run(std::vector<Entity> entities) const override
    {
        if (entities.empty())
            return;

        for (auto entity : entities)
        {
            auto light =
                World::EntityToComponent<DirectionalLightComponent>(entity);

            for (auto shader : light->shaders)
            {
                if (Shader::GetId(shader) == (unsigned int) 0)
                {
                    ERROR("Light shader not found with name: ", shader);
                    continue;
                }
                Shader::Use(shader);

                /* Set the light properties */
                Shader::SetVec3(shader, "dirLight.direction", light->direction);
                Shader::SetVec3(shader, "dirLight.ambient", light->ambient);
                Shader::SetVec3(shader, "dirLight.diffuse", light->diffuse);
                Shader::SetVec3(shader, "dirLight.specular", light->specular);
                Shader::SetFloat(shader, "dirLight.dir_strength",
                                 light->strength);
                Shader::SetBool(shader, "useDirLight", true);
            }
        }
    }
};
