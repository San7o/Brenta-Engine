#include <iostream>
#include "world.h"

using namespace ECS;

int main() {

    World::Init();
    std::cout << "Welcome to my Game!" << std::endl;

    /* New entity */
    Entity entity = World::NewEntity();

    /* Add a health component to the entity */
    struct Health : Component {
        int value;
        Health(Entity entity, int value)
                : Component(entity, "Health"), value(value) {}
    };
    auto health = std::make_shared<Health>(entity, 100);
    World::AddComponent(health);

    /* System to decrease health each tick */
    auto poison = std::make_shared<System>("Poison", []() {
        auto h = static_cast<Health*>
                          (World::getComponents()->at("Health")
                           .at(0).get());
        h->value--;
        std::cout << "Health: " << h->value << std::endl;
    });
    World::AddSystem(poison);

    /* Main loop */
    for(int i = 0; i < 10; i++) {
        World::Tick();
    }

    World::Delete();
    return 0;
}
