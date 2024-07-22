#pragma once

#ifndef DIRECTIONAL_LIGHT_COMPONENT_H
#define DIRECTIONAL_LIGHT_COMPONENT_H

#include "engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace ECS;

struct DirectionalLightComponent : Types::Component {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float strength;
    std::vector<Types::ShaderName> shaders;
    DirectionalLightComponent(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
                              glm::vec3 specular, float strength,
                              std::vector<Types::ShaderName> shaders)
            : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular),
            strength(strength), shaders(shaders) {}
};

#endif
