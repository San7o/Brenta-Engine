#pragma once

#include "engine.h"
#include "ecs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace ECS;

struct DirectionalLightComponent : Component {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float strength;
    std::vector<Types::ShaderName> shaders;
    DirectionalLightComponent() : direction(glm::vec3(0.0f)), ambient(glm::vec3(0.0f)),
            diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)), strength(0.0f) {}
    DirectionalLightComponent(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
                              glm::vec3 specular, float strength,
                              std::vector<ShaderName> shaders)
            : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular),
            strength(strength), shaders(shaders) {}
};
