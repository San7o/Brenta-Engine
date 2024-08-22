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
 * This file contains the definition of the World class, which is a singleton
 * that contains all the entities, components and systems in the game world,
 * provides acces to them through queries and is responsible for updating the
 * game world.
 */

#pragma once

#include "ecs.h"
#include "engine_logger.h"
#include "ecs_types.h"

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <algorithm>

namespace ECS {

using namespace Types;

class World {
public:

World() = delete;

static void Init();
static void Delete();
static void Tick();

static std::set<Entity>*                    getEntities();
static UMap<std::type_index, Resource>*     getResources();
static UMapVec<std::type_index, Component>* getComponents();

static Entity NewEntity();

template <typename R>
static R* GetResource()
{
    if (!resources) {
        Logger::Log(LogLevel::ERROR, "Cannot get resource: world not initialized");
        return nullptr;
    }

    auto ret = resources->at(std::type_index(typeid(R)));
    if (ret) return static_cast<R*>(ret.get());
    return nullptr;
}

template <typename C>
static void AddComponent(Entity entity, C new_component)
{
    if (!components) {
        Logger::Log(LogLevel::ERROR, "Cannot add component: world not initialized");
        return;
    }

    auto component = std::make_shared<C>(new_component);
    
    component->entity = entity;

    if (!components->count(std::type_index(typeid(C)))) {
        components->insert({std::type_index(typeid(C)), {component}});
    }
    else {
        components->at(std::type_index(typeid(C))).push_back(component);
    }

    Logger::Log(LogLevel::INFO, "Added component: ", std::type_index(typeid(C)).name());
}


template <typename R>
static void AddResource(R resource)
{
    if (!resources) {
        Logger::Log(LogLevel::ERROR, "Cannot add resource: world not initialized");
        return;
    }

    resources->insert({std::type_index(typeid(R)), std::make_shared<R>(resource)});
    Logger::Log(LogLevel::INFO, "Added Resource: ", std::type_index(typeid(R)).name());
}

static void RemoveEntity(Entity entity);

template <typename C>
static C* EntityToComponent(Entity entity)
{
    if (!components) {
        Logger::Log(LogLevel::ERROR, "Cannot get component: world not initialized");
        return nullptr;
    }

    if (!components->count(std::type_index(typeid(C)))) {
        Logger::Log(LogLevel::ERROR, "Component not found: ", std::type_index(typeid(C)).name());
        return nullptr;
    }

    for (auto component : components->at(std::type_index(typeid(C)))) {
        if (component->entity == entity) {
            return static_cast<C*>(component.get());
        }
    }

    return nullptr;
}

/* Iterate over all systems and run them */
template<typename Tuple, std::size_t... Is>
static void for_each_impl(Tuple&& tuple, std::index_sequence<Is...>) {
    (..., process(std::get<Is>(std::forward<Tuple>(tuple))));
}
template<typename Tuple>
static void for_each(Tuple&& tuple) {
    for_each_impl(std::forward<Tuple>(tuple),
        std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}
template <typename... T>
static std::vector<ECS::Entity> QueryComponentsTuple(std::tuple<T...>) {
    return QueryComponents<T...>();
}
template<typename System>
static void process(const System& system) {
    using Dependencies = typename System::dependencies;
    std::vector<ECS::Entity> matches = QueryComponentsTuple(Dependencies{});
    system.run(matches);
}

static void RunSystems();

private:
static SetPtr<Entity>                         entities;
static UMapPtr<std::type_index, Resource>     resources;
static UMapVecPtr<std::type_index, Component> components;

template <typename C, typename... Components, typename N = None>
static std::vector<Entity> QueryComponents()
{
    if (!World::components) {
        Logger::Log(LogLevel::ERROR, "Cannot query: world not initialized");
        return {};
    }

    std::vector<Entity> matched;

    if (!components->count(std::type_index(typeid(C)))) {
        return matched;
    }

    for (auto component : components->at(std::type_index(typeid(C)))) {
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

    if (!components->count(std::type_index(typeid(C)))) {
        *entities = matched;
        return;
    }
    for (auto component : components->at(std::type_index(typeid(C)))) {
        if (std::find(entities->begin(), entities->end(), (int&) component->entity) != entities->end()) {
            matched.push_back(component->entity);
        }
    }
    *entities = matched;

    if (sizeof...(Components) == 0) return;
    
    (QueryComponentsRec<Components>(entities), ...);
}

};

} // namespace ECS

