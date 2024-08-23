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

#include <typeinfo>
#include <unordered_map>

using namespace Brenta;
using namespace Brenta::ECS;

struct AComponent : Component {
    int payload;
    AComponent() {}
    AComponent(int payload) : payload(payload) {}
};

TEST_CASE("Add a component to an entity")
{
    Logger::SetLogLevel(Brenta::Types::LogLevel::DISABLED);
    World::Init();

    Entity entity = World::NewEntity();
    REQUIRE(entity != -1);

    World::AddComponent<AComponent>(entity, AComponent(69));
    auto components = World::getComponents();
    REQUIRE(components != nullptr);
    REQUIRE(components->size() == 1);
    REQUIRE(components->count(typeid(AComponent)) == 1);
    REQUIRE(components->at(typeid(AComponent)).size() == 1);

    auto type_components = components->at(typeid(AComponent));
    REQUIRE(type_components.size() == 1);

    auto component = static_cast<AComponent*>(type_components[0].get());
    REQUIRE(component != nullptr);
    REQUIRE(component->payload == 69);
    REQUIRE(component->entity == entity);

    World::Delete();
}

TEST_CASE("EntityToComponent")
{
    Logger::SetLogLevel(Brenta::Types::LogLevel::DISABLED);
    World::Init();

    Entity entity = World::NewEntity();
    REQUIRE(entity != -1);

    World::AddComponent<AComponent>(entity, AComponent(69));
    auto component = World::EntityToComponent<AComponent>(entity);
    REQUIRE(component != nullptr);
    REQUIRE(component->payload == 69);
    REQUIRE(component->entity == entity);

    World::Delete();
}
