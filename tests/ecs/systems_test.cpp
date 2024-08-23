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

#include "catch_amalgamated.hpp"
#include "ecs.hpp"
#include "engine_logger.hpp"

using namespace Brenta;
using namespace Brenta::ECS;

/* Used to count how many times SystemC runs */
int runs = 0;

struct ComponentA : Component {
    int payload;
    ComponentA() {}
    ComponentA(int payload) : payload(payload) {}
};
struct ComponentB : Component {
    int payload;
    ComponentB() {}
    ComponentB(int payload) : payload(payload) {}
};
struct ComponentC : Component {
    int payload;
    ComponentC() {}
    ComponentC(int payload) : payload(payload) {}
};

/* This system increases the payload of ComponentA and ComponentB */
struct SystemA : System<ComponentA, ComponentB> {
    void run(std::vector<Entity> matched) const override {
        REQUIRE(matched.size() == 1);
        for (auto e : matched) {
            auto component_a = World::EntityToComponent<ComponentA>(e);
            auto component_b = World::EntityToComponent<ComponentB>(e);
            component_a->payload++;
            component_b->payload++;
        }
    }
};

/* ComponentC is not assigned to any entity */
struct SystemB : System<ComponentC> {
    void run(std::vector<Entity> matched) const override {
        REQUIRE(matched.size() == 0);
    }
};

/* This system has no dependencies and should always run */
struct SystemC : System<None> {
    void run(std::vector<Entity> matched) const override {
        REQUIRE(matched.size() == 0);
        runs++;
    }
};
REGISTER_SYSTEMS(SystemA, SystemB, SystemC);

TEST_CASE("Run some registered systems")
{
    Logger::SetLogLevel(Brenta::Types::LogLevel::DISABLED);
    World::Init();

    Entity e = World::NewEntity();
    World::AddComponent<ComponentA>(e, 69);
    World::AddComponent<ComponentB>(e, 69);

    auto component_a = World::EntityToComponent<ComponentA>(e);
    auto component_b = World::EntityToComponent<ComponentB>(e);
    REQUIRE(component_a->payload == 69);
    REQUIRE(component_b->payload == 69);

    World::Tick();
    REQUIRE(component_a->payload == 70);
    REQUIRE(component_b->payload == 70);

    World::Tick();
    REQUIRE(component_a->payload == 71);
    REQUIRE(component_b->payload == 71);

    World::Tick();
    REQUIRE(component_a->payload == 72);
    REQUIRE(component_b->payload == 72);

    REQUIRE(runs == 3);

    World::Delete();
}
