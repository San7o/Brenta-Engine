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

#include "world.hpp"

#include <iostream>

using namespace Brenta;
using namespace Brenta::ECS;

struct PlayerComponent : Component
{
    PlayerComponent()
    {
    }
};

struct HealthComponent : Component
{
    int value;
    HealthComponent()
    {
    }
    HealthComponent(int value) : value(value)
    {
    }
};

struct PoisonSystem : System<PlayerComponent, HealthComponent>
{
    void run(std::vector<Entity> entities) const override
    {
        if (entities.empty())
        {
            return;
        }

        auto health = World::EntityToComponent<HealthComponent>(entities.at(0));
        health->value--;
        std::cout << "Health: " << health->value << std::endl;
    }
};

struct GlobalResource : Resource
{
    int value;
    GlobalResource()
    {
    }
    GlobalResource(int value) : value(value)
    {
    }
};

REGISTER_SYSTEMS(PoisonSystem);

int main()
{
    World::Init();
    std::cout << "Welcome to my Test Game!" << std::endl;

    /* New entity as the player */
    Entity player_entity = World::NewEntity();

    /* Add the Player component to the entity */
    World::AddComponent<PlayerComponent>(player_entity, PlayerComponent());

    /* Add a health component to the entity */
    auto health_component = HealthComponent(100);
    World::AddComponent<HealthComponent>(player_entity, health_component);

    World::AddResource<GlobalResource>(GlobalResource(10));

    /* Main loop */
    for (int i = 0; i < 10; i++)
    {
        World::Tick();
    }

    World::Delete();
    return 0;
}
