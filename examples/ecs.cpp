/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "viotecs/viotecs.hpp"

#include <iostream>

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

        auto health =
            world::entity_to_component<health_component>(entities.at(0));
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
    std::cout << "You are poisoned and will lose health every tick"
              << std::endl;

    /* New entity as the player */
    entity_t player_entity = world::new_entity();

    /* Add the Player component to the entity */
    world::add_component<player_component>(player_entity, player_component());

    /* Add a health component to the entity */
    auto health = health_component(100);
    world::add_component<health_component>(player_entity, health);

    world::add_resource<global_resource>(global_resource(10));

    /* Main loop */
    for (int i = 0; i < 10; i++)
    {
        world::tick();
    }

    world::destroy();
    return 0;
}
