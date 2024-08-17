#include "callbacks/dash_callback.h"
#include "components/player_component.h"
#include "components/physics_component.h"
#include "engine.h"

using namespace ECS;
using namespace ECS::Types;

#define BOOST 20.0f

void InitDashCallback() {
    auto dash_callback = []() {
        auto matches = World::QueryComponents({"PlayerComponent", "PhysicsComponent"});
        for (auto match : matches) {
            auto player_component = static_cast<PlayerComponent*>
                (World::EntityToComponent(match, "PlayerComponent"));

            auto physics_component = static_cast<PhysicsComponent*>
                (World::EntityToComponent(match, "PhysicsComponent"));

            if (player_component->dashCharge >= player_component->dashTreshold) {
                player_component->dashCharge = 0;
                physics_component->velocity *= glm::vec3(BOOST, BOOST, BOOST);
            }
        }
    };
    Input::AddKeyboardCallback(GLFW_KEY_SPACE, dash_callback);
}
