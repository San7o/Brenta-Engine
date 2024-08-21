#pragma once

#include "systems/physics_system.h"

#include "engine.h"
#include "ecs.h"
#include "components/transform_component.h"
#include "components/physics_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ECS;
using namespace ECS::Types;

struct PhysicsSystem : System<PhysicsComponent, TransformComponent> {

    void run() const override {

        auto matches = World::QueryComponents<PhysicsComponent, TransformComponent>();
        if (matches.empty()) return;

        for (auto match : matches) {

            auto physics_component = static_cast<PhysicsComponent*>
                 (World::EntityToComponent<PhysicsComponent>(match));

            auto transform_component = static_cast<TransformComponent*>
                 (World::EntityToComponent<TransformComponent>(match));
            
            if (physics_component->acceleration != glm::vec3(0.0f)) {
                physics_component->velocity += physics_component->acceleration * Time::GetDeltaTime();
            }
            if (physics_component->velocity != glm::vec3(0.0f)) {
                transform_component->position += physics_component->velocity * Time::GetDeltaTime();
            }
        }
    }
};
