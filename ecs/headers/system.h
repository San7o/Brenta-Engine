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
