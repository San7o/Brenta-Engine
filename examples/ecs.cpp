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

#include <iostream>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct player_component : component
{
  player_component()
  {
  }
};

struct health_component : component
{
  int value;
  health_component()
  {
  }
  health_component(int value) : value(value)
  {
  }
};

struct poison_system : system<player_component, health_component>
{
  void run(std::vector<entity_t> entities) const override
  {
    if (entities.empty())
    {
      return;
    }

    auto health = world::entity_to_component<health_component>(entities.at(0));
    health->value--;
    std::cout << "Health: " << health->value << std::endl;
  }
};

struct global_resource : resource
{
  int value;
  global_resource()
  {
  }
  global_resource(int value) : value(value)
  {
  }
};

REGISTER_SYSTEMS(poison_system);

int main()
{
  world::init();
  std::cout << "Welcome to my Test Game!" << std::endl;
  std::cout << "You are poisoned and will lose health every tick" << std::endl;

  // New entity as the player
  entity_t player_entity = world::new_entity();

  // Add the Player component to the entity
  world::add_component<player_component>(player_entity, player_component());

  // Add a health component to the entity
  auto health = health_component(100);
  world::add_component<health_component>(player_entity, health);

  world::add_resource<global_resource>(global_resource(10));

  // Main loop
  for (int i = 0; i < 10; i++)
  {
    world::tick();
  }

  world::destroy();
  return 0;
}
