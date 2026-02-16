// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// ecs.cpp
// =======
//
// Some ecs examples
//

#include <viotecs/viotecs.hpp>

#include <iostream>

using namespace viotecs;

struct PlayerComponent : Component
{
  PlayerComponent() {}
};

struct HealthComponent : Component
{
  int value;
  HealthComponent() {}
  HealthComponent(int value) : value(value) {}
};

struct PoisonSystem : System<PlayerComponent, HealthComponent>
{
  void run(std::vector<EntityId> entities) const override
  {
    if (entities.empty())
    {
      return;
    }

    auto health = World::entity_to_component<HealthComponent>(entities.at(0));
    health->value--;
    std::cout << "Health: " << health->value << std::endl;
  }
};

struct GlobalResource : Resource
{
  int value;
  GlobalResource() {}
  GlobalResource(int value) : value(value) {}
};

int main()
{
  World::init();
  World::register_systems<PoisonSystem>();
  
  std::cout << "Welcome to my Test Game!" << std::endl;
  std::cout << "You are poisoned and will lose health every tick" << std::endl;

  // New entity as the player
  Entity player = World::new_entity();

  // Add the Player component to the entity
  player.add_component<PlayerComponent>();

  // Add a health component to the entity
  player.add_component<HealthComponent>(100);

  World::add_resource<GlobalResource>(10);

  // Main loop
  for (int i = 0; i < 10; i++)
  {
    World::tick();
  }

  World::destroy();
  return 0;
}
