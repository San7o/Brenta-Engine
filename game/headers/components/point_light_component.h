#pragma once

#include "engine.h"
#include "ecs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace ECS;

struct PointLightComponent : Component {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    // Attenuation
    float constant;  // at least 1
    float linear;
    float quadratic;
    float strength;
    std::vector<Types::ShaderName> shaders;

    PointLightComponent(glm::vec3 ambient, glm::vec3 diffuse,
                    glm::vec3 specular, float constant,
                    float linear, float quadratic, float strength,
                    std::vector<Types::ShaderName> shaders)
            : ambient(ambient), diffuse(diffuse), specular(specular),
            constant(constant), linear(linear), quadratic(quadratic),
            strength(strength), shaders(shaders) {}

};
