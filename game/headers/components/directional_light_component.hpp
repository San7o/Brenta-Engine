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

#include "ecs.hpp"
#include "engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace brenta;
using namespace brenta::ecs;

struct DirectionalLightComponent : component
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float strength;
    std::vector<brenta::types::shader_name_t> shaders;
    DirectionalLightComponent()
        : direction(glm::vec3(0.0f)), ambient(glm::vec3(0.0f)),
          diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)), strength(0.0f)
    {
    }
    DirectionalLightComponent(glm::vec3 direction, glm::vec3 ambient,
                              glm::vec3 diffuse, glm::vec3 specular,
                              float strength,
                              std::vector<brenta::types::shader_name_t> shaders)
        : direction(direction), ambient(ambient), diffuse(diffuse),
          specular(specular), strength(strength), shaders(shaders)
    {
    }
};
