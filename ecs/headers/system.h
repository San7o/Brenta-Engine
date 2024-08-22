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
 * The above copyright notice and this permission notice shall be included in all
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

/*
 * This file contains the definition of the System type.
 *
 * A System is a function that processes game objects and are
 * called at any tick by the World.
 */

#pragma once

#include <typeindex>
#include <vector>
#include <tuple>

namespace ECS {

/**
 * Example System usage:
 *
 *   struct SystemA : System<ComponentA, ComponentB> {
 *     void run (std::vector<Entity> e) const override { std::cout << "A\n"; }
 *   };
 *
 * If no components are needed, use System<None>.
 */
template <typename... T>
struct System {
    using dependencies = std::tuple<T...>;
    virtual void run(std::vector<Entity> e) const {};
};

template<typename... T>
struct RegisteredSystems {
    using systems = std::tuple<T...>;
};

/**
 * Use this macro anywhere (outside a function) to register systems.
 * Use it only once.
 */
#define REGISTER_SYSTEMS(...) \
    void World::RunSystems() { \
        typedef RegisteredSystems<__VA_ARGS__>::systems registered_systems; \
        const registered_systems systems; \
        for_each(systems); \
    }

} // namespace ECS
