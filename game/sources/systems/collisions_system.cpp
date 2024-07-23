#include "systems/collisions_system.h"

#include "engine.h"
#include "components/transform_component.h"
#include "components/physics_component.h"
#include "components/sphere_collider_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ECS;
using namespace ECS::Types;

glm::vec3 ResolveCollision(glm::vec3 position1, glm::vec3 position2,
                           float radius1, float radius2, float distance);

void InitCollisionsSystem() {

    auto collisions_system = std::make_shared<System>("CollisionsSystem", []() {

        auto matches = World::QueryComponents(
                        {"SphereColliderComponent", "TransformComponent"});
        if (matches.empty()) return;

        for (long unsigned int i = 0; i < matches.size(); i++) {
            for (long unsigned int j = 0; j < matches.size(); j++) {
                if (i == j) continue;
                auto sphere_component1 = static_cast<SphereColliderComponent*>
                     (World::EntityToComponent(matches[i], "SphereColliderComponent"));
                auto transform_component1 = static_cast<TransformComponent*>
                     (World::EntityToComponent(matches[i], "TransformComponent"));
                auto sphere_component2 = static_cast<SphereColliderComponent*>
                     (World::EntityToComponent(matches[j], "SphereColliderComponent"));
                auto transform_component2 = static_cast<TransformComponent*>
                     (World::EntityToComponent(matches[j], "TransformComponent"));
                
                float distance = glm::distance(transform_component1->position,
                                               transform_component2->position);
                if (distance < sphere_component1->radius + sphere_component2->radius) {
                    auto physics_component1 = static_cast<PhysicsComponent*>
                         (World::EntityToComponent(matches[i], "PhysicsComponent"));
                    auto physics_component2 = static_cast<PhysicsComponent*>
                         (World::EntityToComponent(matches[j], "PhysicsComponent"));
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
    });
    World::AddSystem(collisions_system);
}

glm::vec3 ResolveCollision(glm::vec3 position1, glm::vec3 position2,
                           float radius1, float radius2, float distance) {
    return position1 + glm::normalize(position1 - position2) * (radius1 + radius2 - distance);

}
