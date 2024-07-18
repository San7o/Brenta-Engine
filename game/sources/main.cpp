#include <iostream>
#include "world.h"

using namespace ECS;

int main() {

    World::Init();
    std::cout << "Welcome to my Game!" << std::endl;

    /* New entity as the player */
    Entity player_entity = World::NewEntity();

    /* Add the Player component to the entity */
    struct Player : Component {
        Player(Entity entity)
                : Component(entity, "Player") {}
    };
    auto playerComponent = std::make_shared<Player>(player_entity);
    World::AddComponent(playerComponent);

    /* Add a health component to the entity */
    struct HealthComponent : Component {
        int value;
        HealthComponent(Entity entity, int value)
                : Component(entity, "Health"), value(value) {}
    };
    auto health_component = std::make_shared<HealthComponent>(player_entity, 100);
    World::AddComponent(health_component);

    /* System to decrease health to the player */
    auto poison = std::make_shared<System>("Poison", []() {
        /* Query entities with both PlayerComponent and HealthComponent */
        auto matches = World::QueryComponents({"Player", "Health"});
        if (matches.empty()) {
            return;
        }

        /* Get health component */
        auto health = static_cast<HealthComponent*>
                     (World::EntityToComponent(matches.at(0), "Health"));

        health->value--;
        std::cout << "Health: " << health->value << std::endl;
    });
    World::AddSystem(poison);

    /* Create a global resource */
    struct GlobalResource : Resource {
        int value;
        GlobalResource(int value) :
                Resource("GlobalResource"), value(value) {}
    };
    World::AddResource(std::make_shared<GlobalResource>(10));

    /* Main loop */
    for(int i = 0; i < 10; i++) {
        World::Tick();
    }

    World::Delete();
    return 0;
}
