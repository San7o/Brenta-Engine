#pragma once

#include "engine.hpp"
#include "ecs.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace ECS;

struct PhysicsComponent : Component {
    float mass;
    float density;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool isElastic;

    PhysicsComponent() : mass(0.0f), density(0.0f), velocity(glm::vec3(0.0f)),
            acceleration(glm::vec3(0.0f)), isElastic(false) {}
    PhysicsComponent(float mass, float density, glm::vec3 velocity,
                    glm::vec3 acceleration, bool isElastic) :
            mass(mass), density(density), velocity(velocity),
            acceleration(acceleration), isElastic(isElastic) {}
};
