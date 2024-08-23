#pragma once

#include "systems/physics_system.hpp"

#include "engine.hpp"
#include "ecs.hpp"
#include "components/transform_component.hpp"
#include "components/physics_component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace Brenta::ECS;
using namespace Brenta::ECS::Types;

struct PhysicsSystem : System<PhysicsComponent, TransformComponent> {

    void run(std::vector<Entity> matches) const override {
        if (matches.empty()) return;

        for (auto match : matches) {

            auto physics_component = World::EntityToComponent<PhysicsComponent>(match);

            auto transform_component = World::EntityToComponent<TransformComponent>(match);
            
            if (physics_component->acceleration != glm::vec3(0.0f)) {
                physics_component->velocity += physics_component->acceleration * Time::GetDeltaTime();
            }
            if (physics_component->velocity != glm::vec3(0.0f)) {
                transform_component->position += physics_component->velocity * Time::GetDeltaTime();
            }
        }
    }
};
