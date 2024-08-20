#pragma once

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "engine.h"
#include "ecs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace ECS;

struct PhysicsComponent : Types::Component {
    float mass;
    float density;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool isElastic;

    PhysicsComponent(float mass, float density, glm::vec3 velocity,
                    glm::vec3 acceleration, bool isElastic) :
            mass(mass), density(density), velocity(velocity),
            acceleration(acceleration), isElastic(isElastic) {}
};

#endif
