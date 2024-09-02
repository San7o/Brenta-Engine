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

#pragma once

#include <tuple>
#include <typeindex>
#include <vector>

namespace Brenta
{

namespace ECS
{

/**
 * @brief System type
 *
 * This type is used to identify systems in the game world. Systems are
 * functions that process game objects and are called at any tick by the World.
 *
 * Systems can query an entity via It's components specified in the template
 * arguments. The system will be called each tick with a list of entities
 * that have all the components specified.
 *
 * Example of creating a system:
 *
 * ```
 * struct SystemA : System<ComponentA, ComponentB> {
 *   void run (std::vector<Entity> e) const override { std::cout << "A\n"; }
 * };
 * ```
 *
 * If no components are needed, use System<None>.
 */
template <typename... T> struct System
{
    using dependencies = std::tuple<T...>;
    virtual void run(std::vector<Entity> e) const {};
};

/**
 * @brief Registered Systems Type
 *
 * This struct is used as a type to register systems in the World.
 * Use the REGISTER_SYSTEMS(...) macro to register systems, this
 * will create an instance of Registeres Systems with the systems
 * specified. This is used to call the systems in the World.
 *
 * Example:
 * ```
 * REGISTERED_SYSTEMS(SystemA, SystemB);
 * ```
 */
template <typename... T> struct RegisteredSystems
{
    using systems = std::tuple<T...>;
};

/**
 * Use this macro anywhere (outside a function) to register systems.
 * Use it only once.
 */
#define REGISTER_SYSTEMS(...)                                                  \
    void Brenta::ECS::World::RunSystems()                                      \
    {                                                                          \
        typedef RegisteredSystems<__VA_ARGS__>::systems registered_systems;    \
        const registered_systems systems;                                      \
        for_each(systems);                                                     \
    }

} // namespace ECS

} // namespace Brenta
