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

using namespace brenta::ecs;

struct DirectionalLightSystem : system<DirectionalLightComponent>
{
    void run(std::vector<entity_t> entities) const override
    {
        if (entities.empty())
            return;

        for (auto entity : entities)
        {
            auto light =
                world::entity_to_component<DirectionalLightComponent>(entity);

            for (auto shader : light->shaders)
            {
                if (shader::get_id(shader) == (unsigned int) 0)
                {
                    ERROR("Light shader not found with name: ", shader);
                    continue;
                }
                shader::use(shader);

                /* Set the light properties */
                shader::set_vec3(shader, "dirLight.direction", light->direction);
                shader::set_vec3(shader, "dirLight.ambient", light->ambient);
                shader::set_vec3(shader, "dirLight.diffuse", light->diffuse);
                shader::set_vec3(shader, "dirLight.specular", light->specular);
                shader::set_float(shader, "dirLight.dir_strength",
                                 light->strength);
                shader::set_bool(shader, "useDirLight", true);
            }
        }
    }
};
