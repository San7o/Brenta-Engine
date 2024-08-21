#include "systems/player_movement_system.h"

#include "engine.h"
#include "components/player_component.h"
#include "components/transform_component.h"
#include "components/physics_component.h"
#include "components/particle_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ACCELERATION 50.0f
#define BOOST 1.5f
#define SCALE 0.0085f

using namespace ECS;
using namespace ECS::Types;

void InitPlayerMovementSystem() {

    auto player_movement = std::make_shared<System>("PlayerMovementSystem", []() {
        /* Get the entities with the model component */
        auto matches = World::QueryComponents({"PlayerComponent"});
        if (matches.empty()) return;

        for (auto match : matches) {
            /* Get the model component */
            auto physics_component = World::EntityToComponent(match, "PhysicsComponent");
          
            auto model_component = World::EntityToComponent(match, "ModelComponent");

            auto player_component = World::EntityToComponent(match, "PlayerComponent");

            auto transform_component = World::EntityToComponent(match, "TransformComponent");

            auto particle_component = World::EntityToComponent(match, "ParticleComponent");

            if (Screen::isKeyPressed(GLFW_KEY_RIGHT)) {
                 physics_component->acceleration += glm::vec3(ACCELERATION, 0.0f, 0.0f);
            }
            if (Screen::isKeyPressed(GLFW_KEY_LEFT)) {
                 physics_component->acceleration += glm::vec3(-ACCELERATION, 0.0f, 0.0f);
            }
            if (Screen::isKeyPressed(GLFW_KEY_UP)) {
                 physics_component->acceleration += glm::vec3(0.0f, ACCELERATION, 0.0f);
            }
            if (Screen::isKeyPressed(GLFW_KEY_DOWN)) {
                 physics_component->acceleration += glm::vec3(0.0f, -ACCELERATION, 0.0f);
            }

            /* Change atlas index */
            const float threshold = 1.0f;
            if (physics_component->velocity.x > threshold) {
                if (physics_component->velocity.y > threshold) {
                    model_component->startingIndex = 2;
                }
                else if (physics_component->velocity.y < -threshold) {
                    model_component->startingIndex = 14;
                }
                else {
                    model_component->startingIndex = 0;
                }
            }
            else if (physics_component->velocity.x < -threshold) {
                if (physics_component->velocity.y > threshold) {
                    model_component->startingIndex = 6;
                }
                else if (physics_component->velocity.y < -threshold) {
                    model_component->startingIndex = 10;
                }
                else {
                    model_component->startingIndex = 8;
                }
            }
            else if (physics_component->velocity.y > threshold) {
                model_component->startingIndex = 4;
            }
            else if (physics_component->velocity.y < -threshold) {
                model_component->startingIndex = 12;
            }


            /* Recharge dash */
            player_component->dashCharge += Time::GetDeltaTime();


            /* Keep the player in the screen */
            if (transform_component->position.x > SCALE * Screen::GetWidth()) {
                transform_component->position.x = SCALE * Screen::GetWidth();
                physics_component->velocity.x = 0.0f;
            }
            if (transform_component->position.x < -SCALE * Screen::GetWidth()) {
                transform_component->position.x = -SCALE * Screen::GetWidth();
                physics_component->velocity.x = 0.0f;
            }
            if (transform_component->position.y > SCALE * Screen::GetHeight()) {
                transform_component->position.y = SCALE * Screen::GetHeight();
                physics_component->velocity.y = 0.0f;
            }
            if (transform_component->position.y < -SCALE * Screen::GetHeight()) {
                transform_component->position.y = -SCALE * Screen::GetHeight();
                physics_component->velocity.y = 0.0f;
            }


            /* Spawn particles */
            particle_component->emitter->starting_position = transform_component->position + glm::vec3(0.0f, 0.0f, -1.0f);
        }
    });

    World::AddSystem(player_movement);
}
