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

/**
 * Examples of various components that can be used in an ECS system.
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

struct PhysicsComponent : component
{
    float mass;
    float density;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool isElastic;

    PhysicsComponent()
        : mass(0.0f), density(0.0f), velocity(glm::vec3(0.0f)),
          acceleration(glm::vec3(0.0f)), isElastic(false)
    {
    }
    PhysicsComponent(float mass, float density, glm::vec3 velocity,
                     glm::vec3 acceleration, bool isElastic)
        : mass(mass), density(density), velocity(velocity),
          acceleration(acceleration), isElastic(isElastic)
    {
    }
};

struct PlayerComponent : component
{
    PlayerComponent()
    {
    }
};

struct TransformComponent : component
{
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;

    TransformComponent()
        : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(1.0f)
    {
    }
    TransformComponent(glm::vec3 position, glm::vec3 rotation, float scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }
};

struct PointLightComponent : component
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    // Attenuation
    float constant; // at least 1
    float linear;
    float quadratic;
    float strength;
    std::vector<brenta::types::shader_name_t> shaders;

    PointLightComponent()
        : ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)),
          specular(glm::vec3(0.0f)), constant(1.0f), linear(0.0f),
          quadratic(0.0f), strength(0.0f)
    {
    }
    PointLightComponent(glm::vec3 ambient, glm::vec3 diffuse,
                        glm::vec3 specular, float constant, float linear,
                        float quadratic, float strength,
                        std::vector<Brenta::Types::ShaderName> shaders)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          constant(constant), linear(linear), quadratic(quadratic),
          strength(strength), shaders(shaders)
    {
    }
};
