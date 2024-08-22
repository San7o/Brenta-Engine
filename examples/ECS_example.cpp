#include <iostream>
#include "world.hpp"

using namespace ECS;

REGISTER_SYSTEMS(PoisonSystem);

int main() {

    World::Init();
    std::cout << "Welcome to my Game!" << std::endl;

    /* New entity as the player */
    Entity player_entity = World::NewEntity();

    /* Add the Player component to the entity */
    struct PlayerComponent : Component {
        PlayerComponent() {}
    };
    World::AddComponent<PlayerComponent>(player_entity, PlayerComponent);

    /* Add a health component to the entity */
    struct HealthComponent : Component {
        int value;
        HealthComponent() {}
        HealthComponent(int value)
                : value(value) {}
    };
    auto health_component = HealthComponent(100);
    World::AddComponent<HealthComponent>(plauer_entity, health_component);

    /* System to decrease health to the player */
    struct PoisonSystem : System<PlayerComponent, HealthComponent> {
        void run(std::vector<Entity> entities) const override {
            if (entities.empty()) {
                return;
            }

            auto health = World::EntityToComponent<HealthComponent>(entities.at(0));
            health->value--;
            std::cout << "Health: " << health->value << std::endl;
        }
    }

    /* Create a global resource */
    struct GlobalResource : Resource {
        int value;
        GlobalResource(int value) :
                Resource("GlobalResource"), value(value) {}
    };
    World::AddResource<GlobalResource>(GlobalResource(10));

    /* Main loop */
    for(int i = 0; i < 10; i++) {
        World::Tick();
    }

    World::Delete();
    return 0;
}
