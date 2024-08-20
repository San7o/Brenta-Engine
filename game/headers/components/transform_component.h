#pragma once

#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "engine.h"
#include "ecs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ECS;

struct TransformComponent : Types::Component {
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;

    TransformComponent(glm::vec3 position, glm::vec3 rotation, float scale)
            : position(position), rotation(rotation), scale(scale) {}
};

#endif
