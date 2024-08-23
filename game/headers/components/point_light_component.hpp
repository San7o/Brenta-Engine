#pragma once

#include "engine.hpp"
#include "ecs.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace Brenta;
using namespace Brenta::ECS;

struct PointLightComponent : Component {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    // Attenuation
    float constant;  // at least 1
    float linear;
    float quadratic;
    float strength;
    std::vector<Brenta::Types::ShaderName> shaders;

    PointLightComponent() : ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)),
            specular(glm::vec3(0.0f)), constant(1.0f), linear(0.0f),
            quadratic(0.0f), strength(0.0f) {}
    PointLightComponent(glm::vec3 ambient, glm::vec3 diffuse,
                    glm::vec3 specular, float constant,
                    float linear, float quadratic, float strength,
                    std::vector<Brenta::Types::ShaderName> shaders)
            : ambient(ambient), diffuse(diffuse), specular(specular),
            constant(constant), linear(linear), quadratic(quadratic),
            strength(strength), shaders(shaders) {}

};
