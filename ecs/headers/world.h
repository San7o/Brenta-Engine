/* 
 * This file contains the definition of the World class, which is a singleton
 * that contains all the entities, components and systems in the game world,
 * provides acces to them through queries and is responsible for updating the
 * game world.
 */

#pragma once

#include "ecs.h"
#include "engine_logger.h"

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <algorithm>

namespace ECS {

namespace Types {

typedef void None;

template <typename T>
using SPtr      = std::shared_ptr<T>;

template <typename T>
using SetPtr     = std::unique_ptr<std::set<T>>;

template <typename T, typename G>
using UMap       = std::unordered_map<T, SPtr<G>>;

template <typename T, typename G>
using UMapVec    = std::unordered_map<T, std::vector<SPtr<G>>>;

template <typename T, typename G>
using UMapPtr    = std::unique_ptr<UMap<T, G>>;

template <typename T, typename G>
using UMapVecPtr = std::unique_ptr<UMapVec<T, G>>;

template <typename T>
using VecSPtr    = std::shared_ptr<std::vector<SPtr<T>>>;

} // namespace Types

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
static std::vector<SPtr<System>>*           getSystems();

static Entity NewEntity();

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

template <typename R>
static Resource* GetResource()
{
    if (!resources) {
        Logger::Log(LogLevel::ERROR, "Cannot get resource: world not initialized");
        return nullptr;
    }

    auto ret = resources->at(std::type_index(typeid(R)));
    if (ret) return ret.get();
    return nullptr;
}

static void AddSystem(SPtr<System> system);

template <typename C>
static void AddComponent(Entity entity, SPtr<Component> component)
{
    if (!components) {
        Logger::Log(LogLevel::ERROR, "Cannot add component: world not initialized");
        return;
    }
    
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
static void AddResource(SPtr<Resource> resource)
{
    if (!resources) {
        Logger::Log(LogLevel::ERROR, "Cannot add resource: world not initialized");
        return;
    }

    resources->insert({std::type_index(typeid(R)), resource});
    Logger::Log(LogLevel::INFO, "Added Resource: ", std::type_index(typeid(R)).name());
}

static void RemoveEntity(Entity entity);

template <typename C>
static Component* EntityToComponent(Entity entity)
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
            return component.get();
        }
    }

    return nullptr;
}

private:
static SetPtr<Entity>     entities;
static VecSPtr<System>    systems;
static UMapPtr<std::type_index, Resource>     resources;
static UMapVecPtr<std::type_index, Component> components;

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
