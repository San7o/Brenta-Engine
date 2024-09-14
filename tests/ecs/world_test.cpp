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

using namespace brenta;
using namespace brenta::ecs;

TEST_CASE("Get ECS without inizialization")
{
    logger::set_log_level(brenta::types::log_level::DISABLED);

    SECTION("Get ECS world entities without inizialization")
    {
        auto entities = world::get_entities();
        REQUIRE(entities == nullptr);
    }

    SECTION("Get ECS world resources without inizialization")
    {
        auto resources = world::get_resources();
        REQUIRE(resources == nullptr);
    }

    SECTION("Get ECS world components without inizialization")
    {
        auto components = world::get_components();
        REQUIRE(components == nullptr);
    }
}

TEST_CASE("ECS world inizialization and destruction")
{
    world::init();

    SECTION("Get ECS world entities after inizialization")
    {
        auto entities = world::get_entities();
        REQUIRE(entities != nullptr);
    }

    SECTION("Get ECS world resources after inizialization")
    {
        auto resources = world::get_resources();
        REQUIRE(resources != nullptr);
    }

    SECTION("Get ECS world components after inizialization")
    {
        auto components = world::get_components();
        REQUIRE(components != nullptr);
    }

    world::destroy();

    SECTION("Get ECS world entities after destruction")
    {
        auto entities = world::get_entities();
        REQUIRE(entities == nullptr);
    }

    SECTION("Get ECS world resources after destruction")
    {
        auto resources = world::get_resources();
        REQUIRE(resources == nullptr);
    }

    SECTION("Get ECS world components after destruction")
    {
        auto components = world::get_components();
        REQUIRE(components == nullptr);
    }

    world::destroy();
}
