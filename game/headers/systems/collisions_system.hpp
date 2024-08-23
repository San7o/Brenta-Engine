#pragma once

#include "systems/collisions_system.hpp"

#include "engine.hpp"
#include "ecs.hpp"
#include "components/transform_component.hpp"
#include "components/physics_component.hpp"
#include "components/sphere_collider_component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace Brenta::ECS;
using namespace Brenta::ECS::Types;

glm::vec3 ResolveCollision(glm::vec3 position1, glm::vec3 position2,
                           float radius1, float radius2, float distance);

struct CollisionsSystem : System<SphereColliderComponent, TransformComponent> {

    void run(std::vector<Entity> matches) const override {
        if (matches.empty()) return;

        for (long unsigned int i = 0; i < matches.size(); i++) {
            for (long unsigned int j = 0; j < matches.size(); j++) {
                if (i == j) continue;
                auto sphere_component1    = World::EntityToComponent<SphereColliderComponent>(matches[i]);
                auto transform_component1 = World::EntityToComponent<TransformComponent>(matches[i]);
                auto sphere_component2    = World::EntityToComponent<SphereColliderComponent>(matches[j]);
                auto transform_component2 = World::EntityToComponent<TransformComponent>(matches[j]);
                
                float distance = glm::distance(transform_component1->position,
                                               transform_component2->position);
                if (distance < sphere_component1->radius + sphere_component2->radius) {
                    auto physics_component1 = World::EntityToComponent<PhysicsComponent>(matches[i]);
                    auto physics_component2 = World::EntityToComponent<PhysicsComponent>(matches[j]);
                    if (physics_component1 == nullptr || physics_component2 == nullptr) {
                        continue;
                    }
                    if (physics_component1 != nullptr && physics_component2 == nullptr) {
                        transform_component1->position = ResolveCollision(transform_component1->position,
                                         transform_component2->position,
                                         sphere_component1->radius, sphere_component2->radius,
                                         distance);
                        physics_component1->velocity = glm::vec3(0.0f);
                    } else if (physics_component1 == nullptr && physics_component2 != nullptr) {
                        transform_component2->position = ResolveCollision(transform_component2->position,
                                         transform_component1->position,
                                         sphere_component2->radius, sphere_component1->radius,
                                         distance);
                        physics_component2->velocity = glm::vec3(0.0f);
                    } else {
                        transform_component1->position = ResolveCollision(transform_component1->position,
                                         transform_component2->position,
                                         sphere_component1->radius, sphere_component2->radius,
                                         distance);
                        transform_component2->position = ResolveCollision(transform_component2->position,
                                         transform_component1->position,
                                         sphere_component2->radius, sphere_component1->radius,
                                         distance);
                        physics_component1->velocity = -physics_component1->velocity;
                        physics_component2->velocity = -physics_component2->velocity;
                    }
                }
            }
        }
    }
};

glm::vec3 ResolveCollision(glm::vec3 position1, glm::vec3 position2,
                           float radius1, float radius2, float distance) {
    return position1 + glm::normalize(position1 - position2) * (radius1 + radius2 - distance);

}
