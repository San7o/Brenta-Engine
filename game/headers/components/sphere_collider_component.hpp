#pragma once

#include "ecs.hpp"
#include "engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Brenta;
using namespace Brenta::ECS;

struct SphereColliderComponent : Component
{
    float radius;
    SphereColliderComponent () : radius (1.0f)
    {
    }
    SphereColliderComponent (float radius) : radius (radius)
    {
    }
};
