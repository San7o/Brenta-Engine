#pragma once

#ifndef SPHERE_COLLIDER_COMPONENT_H
#define SPHERE_COLLIDER_COMPONENT_H

#include "engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ECS;

struct SphereColliderComponent : Types::Component {
    float radius;
    SphereColliderComponent(float radius) : radius(radius) {}
};

#endif
