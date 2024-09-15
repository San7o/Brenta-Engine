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

#include "ecs.hpp"
#include "ecs_types.hpp"
#include "engine_logger.hpp"

#include <algorithm>
#include <memory>
#include <set>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace brenta
{

namespace ecs
{

using namespace types;

/**
 * @brief World class
 *
 * This class is a singleton that contains all the entities, components and
 * systems in the game world, provides acces to them through queries and is
 * responsible for updating the game world.
 */
class world
{
  public:
    world() = delete;

    /**
     * @brief Initialize the world
     *
     * This method initializes the world, creating the entities, components,
     * and resources containers.
     */
    static void init();
    /**
     * @brief Delete the world
     *
     * This method deletes the world, freeing the entities, components,
     * and resources containers, freeing the memory.
     */
    static void destroy();
    /**
     * @brief Tick the world
     *
     * This method ticks the world, calling each system in the world.
     */
    static void tick();

    /**
     * @brief Get the entities container
     * @return The entities container
     */
    static std::set<entity_t> *get_entities();
    /**
     * @brief Get the resources container
     * @return The resources container
     */
    static UMap<std::type_index, resource> *get_resources();
    /**
     * @brief Get the components container
     * @return The components container
     */
    static UMapVec<std::type_index, component> *get_components();

    /**
     * @brief Create a new entity
     * @return The new entity
     */
    static entity_t new_entity();

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
     * auto resource = world::get_resource<shader>();
     * ```
     */
    template <typename R> static R *get_resource()
    {
        if (!resources)
        {
            ERROR("Cannot get resource: world not initialized");
            return nullptr;
        }

        if (!resources->count(std::type_index(typeid(R))))
        {
            ERROR("Resource not found: {}", std::type_index(typeid(R)).name());
            return nullptr;
        }

        auto ret = resources->at(std::type_index(typeid(R)));
        if (ret)
            return static_cast<R *>(ret.get());
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
     * world::add_component<position>(entity, {0, 0, 0});
     * ```
     */
    template <typename C>
    static void add_component(entity_t entity, C new_component)
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

        INFO("Added component: {}", std::type_index(typeid(C)).name());
    }

    /**
     * @brief Add a resource to the world
     *
     * This method adds a resource to the world.
     *
     * Example:
     * ```
     * world::add_resource<shader_res>({shader});
     * ```
     */
    template <typename R> static void add_resource(R resource)
    {
        if (!resources)
        {
            ERROR("Cannot add resource: world not initialized");
            return;
        }

        resources->insert(
            {std::type_index(typeid(R)), std::make_shared<R>(resource)});
        INFO("Added Resource: {}", std::type_index(typeid(R)).name());
    }

    /**
     * @brief Remove an entity
     *
     * This method removes an entity from the world, deleting all its
     * components.
     *
     * @param entity The entity to remove
     *
     * Example:
     * ```
     * world::remove_entity(entity);
     * ```
     */
    static void remove_entity(entity_t entity);

    /**
     * @brief Remove a resource
     *
     * This method removes a resource of the specified type from the world.
     *
     * @tparam R The type of the resource
     *
     * Example:
     * ```
     * World::RemoveResource<Shader>();
     * ```
     */
    template <typename R> static void remove_resource()
    {
        if (!resources)
        {
            ERROR("Cannot remove resource: world not initialized");
            return;
        }

        if (!resources->count(std::type_index(typeid(R))))
        {
            ERROR("Resource not found: {}", std::type_index(typeid(R)).name());
            return;
        }

        resources->erase(std::type_index(typeid(R)));
    }

    /**
     * @brief Get the component of an entity
     *
     * This method returns a pointer to the component of the specified type
     * of the specified entity. If the component is not found, it returns
     * nullptr.
     *
     * @tparam C The type of the component
     * @param entity The entity to get the component from
     * @return A pointer to the component
     *
     * Example:
     * ```
     * auto component = world::entity_to_component<position_comp>(entity);
     * ```
     */
    template <typename C> static C *entity_to_component(entity_t entity)
    {
        if (!components)
        {
            ERROR("Cannot get component: world not initialized");
            return nullptr;
        }

        if (!components->count(std::type_index(typeid(C))))
        {
            ERROR("Component not found: {}", std::type_index(typeid(C)).name());
            return nullptr;
        }

        for (auto component : components->at(std::type_index(typeid(C))))
        {
            if (component->entity == entity)
            {
                return static_cast<C *>(component.get());
            }
        }

        return nullptr;
    }

    /**
     * @brief Run all systems
     *
     * This method runs all the systems in the world.
     */
    static void run_systems();

  private:
    static SetPtr<ecs::entity_t> entities;
    static UMapPtr<std::type_index, resource> resources;
    static UMapVecPtr<std::type_index, component> components;

    /* Iterate over all systems and run them */
    template <typename Tuple, std::size_t... Is>
    static void for_each_impl(Tuple &&tuple, std::index_sequence<Is...>)
    {
        (..., process(std::get<Is>(std::forward<Tuple>(tuple))));
    }
    template <typename Tuple> static void for_each(Tuple &&tuple)
    {
        for_each_impl(std::forward<Tuple>(tuple),
                      std::make_index_sequence<
                          std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
    }
    template <typename... T>
    static std::vector<ecs::entity_t> query_components_tuple(std::tuple<T...>)
    {
        return query_components<T...>();
    }
    template <typename System> static void process(const System &system)
    {
        using dependencies = typename System::dependencies;
        std::vector<ecs::entity_t> matches =
            query_components_tuple(dependencies{});
        system.run(matches);
    }

    template <typename C, typename... Components, typename N = none>
    static std::vector<entity_t> query_components()
    {
        if (!world::components)
        {
            ERROR("Cannot query: world not initialized");
            return {};
        }

        std::vector<entity_t> matched;

        if (!components->count(std::type_index(typeid(C))))
        {
            return matched;
        }

        for (auto component : components->at(std::type_index(typeid(C))))
        {
            matched.push_back(component->entity);
        }

        if (matched.empty())
            return matched;
        if (sizeof...(Components) == 0)
            return matched;

        query_components_rec<Components..., none>(&matched);

        return matched;
    }

    template <typename C, typename... Components>
    static void query_components_rec(std::vector<entity_t> *entities)
    {
        if (entities->empty())
            return;
        std::vector<entity_t> matched;

        if (!components->count(std::type_index(typeid(C))))
        {
            *entities = matched;
            return;
        }
        for (auto component : components->at(std::type_index(typeid(C))))
        {
            if (std::find(entities->begin(), entities->end(),
                          (int &) component->entity)
                != entities->end())
            {
                matched.push_back(component->entity);
            }
        }
        *entities = matched;

        if (sizeof...(Components) == 0)
            return;

        (query_components_rec<Components>(entities), ...);
    }
};

} // namespace ecs

} // namespace brenta
