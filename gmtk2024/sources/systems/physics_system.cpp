#include "systems/physics_system.h"

#include "engine.h"
#include "components/transform_component.h"
#include "components/physics_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define FRICTION 0.3f

using namespace ECS;
using namespace ECS::Types;

void InitPhysicsSystem() {

    auto physics_system = std::make_shared<System>("PhysicsSystem", []() {

        auto matches = World::QueryComponents(
                        {"PhysicsComponent", "TransformComponent"});
        if (matches.empty()) return;

        for (auto match : matches) {

            auto physics_component = static_cast<PhysicsComponent*>
                 (World::EntityToComponent(match, "PhysicsComponent"));

            auto transform_component = static_cast<TransformComponent*>
                 (World::EntityToComponent(match, "TransformComponent"));
            
            if (physics_component->acceleration != glm::vec3(0.0f)) {
                physics_component->velocity += physics_component->acceleration * Time::GetDeltaTime();
            }
            if (physics_component->velocity != glm::vec3(0.0f)) {
                transform_component->position += physics_component->velocity * Time::GetDeltaTime();
            }
            
            /* Global friction */
            if (physics_component->velocity != glm::vec3(0.0f)) {
                physics_component->acceleration -= physics_component->acceleration *  FRICTION;
                physics_component->velocity -= physics_component->velocity* FRICTION;
            }
        }
    });

    World::AddSystem(physics_system);
}
