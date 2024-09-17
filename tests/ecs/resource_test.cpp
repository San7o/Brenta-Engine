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

#include "valfuzz/valfuzz.hpp"
#include "ecs.hpp"

#include <typeinfo>
#include <unordered_map>

using namespace brenta;
using namespace brenta::ecs;

struct TestResource : resource {
    int payload;
    TestResource(int payload) : payload(payload) {}
};

TEST(res, "Add, get and remove a resource from the world")
{
    world::init();

    /* add the resource */
    TestResource resource = TestResource(69);
    world::add_resource<TestResource>(resource);
    
    /* get the resources */
    auto resources = world::get_resources();
    ASSERT(resources != nullptr);
    ASSERT(resources->size() == 1);
    ASSERT(resources->count(typeid(TestResource)) == 1);

    /* get the resource */
    auto my_resource = world::get_resource<TestResource>();
    ASSERT(my_resource != nullptr);
    ASSERT(my_resource->payload == 69);

    /* remove the resource */
    world::remove_resource<TestResource>();
    ASSERT(resources->size() == 0);
    ASSERT(resources->count(typeid(TestResource)) == 0);
    
    /* get the deleted resource */
    my_resource = world::get_resource<TestResource>();
    ASSERT(my_resource == nullptr);

    world::destroy();
}

TEST(no_res, "Get an unexisting resource")
{
    world::init();

    /* get the resource */
    auto my_resource = world::get_resource<TestResource>();
    ASSERT(my_resource == nullptr);

    world::destroy();
}
