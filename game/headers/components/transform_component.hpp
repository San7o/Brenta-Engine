#pragma once

#include "engine.hpp"
#include "ecs.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Brenta;
using namespace Brenta::ECS;

struct TransformComponent : Component {
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;

    TransformComponent() : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(1.0f) {}
    TransformComponent(glm::vec3 position, glm::vec3 rotation, float scale)
            : position(position), rotation(rotation), scale(scale) {}
};
