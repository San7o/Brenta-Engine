#pragma once

#include "engine.h"
#include "ecs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ECS;

struct SphereColliderComponent : Component {
    float radius;
    SphereColliderComponent(float radius) : radius(radius) {}
};
