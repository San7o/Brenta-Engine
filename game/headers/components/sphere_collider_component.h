#pragma once

#include "engine.h"
#include "ecs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ECS;

struct SphereColliderComponent : Component {
    float radius;
    SphereColliderComponent() : radius(1.0f) {}
    SphereColliderComponent(float radius) : radius(radius) {}
};
