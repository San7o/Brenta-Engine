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

#include "components/point_light_component.hpp"
#include "components/transform_component.hpp"
#include "ecs.hpp"
#include "systems/point_lights_system.hpp"

#include <vector>

using namespace brenta::ecs;

/* Load the lights on the shaders */
struct PointLightsSystem : system<TransformComponent, PointLightComponent>
{
    void run(std::vector<entity_t> entities) const override
    {
        if (entities.empty())
            return;

        int counter = 0;
        for (auto entity : entities)
        {
            if (counter >= 4)
            {
                ERROR("Only 4 lights are supported");
                break;
            }
            auto transform =
                world::entity_to_component<TransformComponent>(entity);

            auto light = world::entity_to_component<PointLightComponent>(entity);

            for (auto shader : light->shaders)
            {
                if (shader::get_id(shader) == (unsigned int) 0)
                {
                    ERROR("Light shader not found with name: ", shader);
                    continue;
                }
                shader::use(shader);

                /* Set the light properties */
                std::string lightn =
                    "pointLights[" + std::to_string(counter) + "]";
                shader::set_vec3(shader, (lightn + ".ambient").c_str(),
                                light->ambient);
                shader::set_vec3(shader, (lightn + ".diffuse").c_str(),
                                light->diffuse);
                shader::set_vec3(shader, (lightn + ".specular").c_str(),
                                light->specular);
                shader::set_float(shader, (lightn + ".constant").c_str(),
                                 light->constant);
                shader::set_float(shader, (lightn + ".linear").c_str(),
                                 light->linear);
                shader::set_float(shader, (lightn + ".quadratic").c_str(),
                                 light->quadratic);
                shader::set_float(shader, (lightn + ".strength").c_str(),
                                 light->strength);
                shader::set_vec3(shader, (lightn + ".position").c_str(),
                                transform->position);
                shader::set_float(shader, (lightn + ".point_strength").c_str(),
                                 light->strength);
                shader::set_int(shader, "nPointLights", entities.size());
            }
            counter++;
        }
    }
};
