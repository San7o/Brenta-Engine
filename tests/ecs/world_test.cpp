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
#include "world.hpp"
#include "engine_logger.hpp"

using namespace Brenta;
using namespace Brenta::ECS;

TEST_CASE("Get ECS without inizialization")
{
    Logger::SetLogLevel(Brenta::Types::LogLevel::DISABLED);

    SECTION("Get ECS world entities without inizialization")
    {
        auto entities = World::getEntities();
        REQUIRE(entities == nullptr);
    }

    SECTION("Get ECS world resources without inizialization")
    {
        auto resources = World::getResources();
        REQUIRE(resources == nullptr);
    }

    SECTION("Get ECS world components without inizialization")
    {
        auto components = World::getComponents();
        REQUIRE(components == nullptr);
    }
}

TEST_CASE("ECS world inizialization and destruction")
{
    World::Init();

    SECTION("Get ECS world entities after inizialization")
    {
        auto entities = World::getEntities();
        REQUIRE(entities != nullptr);
    }

    SECTION("Get ECS world resources after inizialization")
    {
        auto resources = World::getResources();
        REQUIRE(resources != nullptr);
    }

    SECTION("Get ECS world components after inizialization")
    {
        auto components = World::getComponents();
        REQUIRE(components != nullptr);
    }

    World::Delete();

    SECTION("Get ECS world entities after destruction")
    {
        auto entities = World::getEntities();
        REQUIRE(entities == nullptr);
    }

    SECTION("Get ECS world resources after destruction")
    {
        auto resources = World::getResources();
        REQUIRE(resources == nullptr);
    }

    SECTION("Get ECS world components after destruction")
    {
        auto components = World::getComponents();
        REQUIRE(components == nullptr);
    }
}
