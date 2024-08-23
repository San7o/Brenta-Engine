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

#pragma once

#include "ecs.hpp"
#include "engine_logger.hpp"
#include "ecs_types.hpp"

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <algorithm>

namespace Brenta
{

namespace ECS
{

using namespace Types;
using namespace Brenta::Utils;

/**
 * @brief World class
 *
 * This class is a singleton that contains all the entities, components and
 * systems in the game world, provides acces to them through queries and is
 * responsible for updating the game world.
 */
class World
{
public:

World() = delete;

/**
 * @brief Initialize the world
 *
 * This method initializes the world, creating the entities, components,
 * and resources containers.
 */
static void Init();
/**
 * @brief Delete the world
 *
 * This method deletes the world, freeing the entities, components,
 * and resources containers, freeing the memory.
 */
static void Delete();
/**
 * @brief Tick the world
 *
 * This method ticks the world, calling each system in the world.
 */
static void Tick();

/**
 * @brief Get the entities container
 * @return The entities container
 */
static std::set<Entity>*                    getEntities();
/**
 * @brief Get the resources container
 * @return The resources container
 */
static UMap<std::type_index, Resource>*     getResources();
/**
 * @brief Get the components container
 * @return The components container
 */
static UMapVec<std::type_index, Component>* getComponents();

/**
 * @brief Create a new entity
 * @return The new entity
 */
static Entity NewEntity();

/**
 * @brief Get a pointer to a resource
 *
 * This method returns a pointer to a resource of the specified type.
 * If the resource is not found, it returns nullptr.
 *
 * @tparam R The type of the resource
 * @return A pointer to the resource
 *
 * Example:
 * ```
 * auto resource = World::GetResource<Shader>();
 * ```
 */
template <typename R>
static R* GetResource()
{
    if (!resources)
    {
        ERROR("Cannot get resource: world not initialized");
        return nullptr;
    }

    auto ret = resources->at(std::type_index(typeid(R)));
    if (ret) return static_cast<R*>(ret.get());
    return nullptr;
}

/**
 * @brief Add a component to an entity
 *
 * This method adds a component to an entity. The component is copied
 * and stored in the world.
 *
 * @tparam C The type of the component
 * @param entity The entity to add the component to
 * @param new_component The component to add
 *
 * Example:
 * ```
 * World::AddComponent<Position>(entity, {0, 0, 0});
 * ```
 */
template <typename C>
static void AddComponent(Entity entity, C new_component)
{
    if (!components)
    {
        ERROR("Cannot add component: world not initialized");
        return;
    }

    auto component = std::make_shared<C>(new_component);
    
    component->entity = entity;

    if (!components->count(std::type_index(typeid(C))))
    {
        components->insert({std::type_index(typeid(C)), {component}});
    }
    else
    {
        components->at(std::type_index(typeid(C))).push_back(component);
    }

    INFO("Added component: ", std::type_index(typeid(C)).name());
}

/**
 * @brief Add a resource to the world
 *
 * This method adds a resource to the world.
 *
 * Example:
 * ```
 * World::AddResource<Shader>({shader});
 * ```
 */
template <typename R>
static void AddResource(R resource)
{
    if (!resources)
    {
        ERROR("Cannot add resource: world not initialized");
        return;
    }

    resources->insert({std::type_index(typeid(R)), std::make_shared<R>(resource)});
    INFO("Added Resource: ", std::type_index(typeid(R)).name());
}

/**
 * @brief Remove an entity
 *
 * This method removes an entity from the world, deleting all its components.
 *
 * @param entity The entity to remove
 *
 * Example:
 * ```
 * World::RemoveEntity(entity);
 * ```
 */
static void RemoveEntity(Entity entity);

/**
 * @brief Get the component of an entity
 *
 * This method returns a pointer to the component of the specified type
 * of the specified entity. If the component is not found, it returns nullptr.
 *
 * @tparam C The type of the component
 * @param entity The entity to get the component from
 * @return A pointer to the component
 *
 * Example:
 * ```
 * auto component = World::EntityToComponent<Position>(entity);
 * ```
 */
template <typename C>
static C* EntityToComponent(Entity entity)
{
    if (!components)
    {
        ERROR("Cannot get component: world not initialized");
        return nullptr;
    }

    if (!components->count(std::type_index(typeid(C))))
    {
        ERROR("Component not found: ", std::type_index(typeid(C)).name());
        return nullptr;
    }

    for (auto component : components->at(std::type_index(typeid(C))))
    {
        if (component->entity == entity)
        {
            return static_cast<C*>(component.get());
        }
    }

    return nullptr;
}

/**
 * @brief Run all systems
 *
 * This method runs all the systems in the world.
 */
static void RunSystems();

private:
static SetPtr<Entity>                         entities;
static UMapPtr<std::type_index, Resource>     resources;
static UMapVecPtr<std::type_index, Component> components;

/* Iterate over all systems and run them */
template<typename Tuple, std::size_t... Is>
static void for_each_impl(Tuple&& tuple, std::index_sequence<Is...>)
{
    (..., process(std::get<Is>(std::forward<Tuple>(tuple))));
}
template<typename Tuple>
static void for_each(Tuple&& tuple) {
    for_each_impl(std::forward<Tuple>(tuple),
        std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}
template <typename... T>
static std::vector<ECS::Entity> QueryComponentsTuple(std::tuple<T...>)
{
    return QueryComponents<T...>();
}
template<typename System>
static void process(const System& system)
{
    using Dependencies = typename System::dependencies;
    std::vector<ECS::Entity> matches = QueryComponentsTuple(Dependencies{});
    system.run(matches);
}

template <typename C, typename... Components, typename N = None>
static std::vector<Entity> QueryComponents()
{
    if (!World::components)
    {
        ERROR("Cannot query: world not initialized");
        return {};
    }

    std::vector<Entity> matched;

    if (!components->count(std::type_index(typeid(C))))
    {
        return matched;
    }

    for (auto component : components->at(std::type_index(typeid(C))))
    {
        matched.push_back(component->entity);
    }

    if (matched.empty()) return matched;
    if (sizeof...(Components) == 0) return matched;

    QueryComponentsRec<Components..., None>(&matched);

    return matched;
}

template <typename C, typename... Components>
static void QueryComponentsRec(std::vector<Entity>* entities)
{
    if (entities->empty()) return;
    std::vector<Entity> matched;

    if (!components->count(std::type_index(typeid(C))))
    {
        *entities = matched;
        return;
    }
    for (auto component : components->at(std::type_index(typeid(C))))
    {
        if (std::find(entities->begin(), entities->end(), (int&) component->entity) != entities->end())
        {
            matched.push_back(component->entity);
        }
    }
    *entities = matched;

    if (sizeof...(Components) == 0) return;
    
    (QueryComponentsRec<Components>(entities), ...);
}

};

} // namespace ECS

} // namespace Brenta
