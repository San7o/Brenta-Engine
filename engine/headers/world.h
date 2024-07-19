/* 
 * This file contains the definition of the World class, which is a singleton
 * that contains all the entities, components and systems in the game world,
 * provides acces to them through queries and is responsible for updating the
 * game world.
 */

#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "component.h"
#include "system.h"
#include "resource.h"

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>

namespace ECS {

namespace Types {

struct CompareSharedPtr {
    bool operator()(const std::shared_ptr<System>& lhs,
                    const std::shared_ptr<System>& rhs) const 
    {
        return lhs->name < rhs->name;
    }
};

template <typename T>
using SPtr      = std::shared_ptr<T>;

template <typename T>
using SetPtr     = std::unique_ptr<std::set<T>>;

template <typename T>
using SetSPtr    = std::unique_ptr<std::set<SPtr<T>, CompareSharedPtr>>;

template <typename T, typename G>
using UMap       = std::unordered_map<T, SPtr<G>>;

template <typename T, typename G>
using UMapVec    = std::unordered_map<T, std::vector<SPtr<G>>>;
template <typename T, typename G>
using UMapPtr    = std::unique_ptr<UMap<T, G>>;

template <typename T, typename G>
using UMapVecPtr = std::unique_ptr<UMapVec<T, G>>;

} // namespace Types

using namespace Types;

class World {
public:
    World() = delete;

    static void Init();
    static void Delete();
    static void Tick();
    static std::vector<Entity> QueryComponents
                      (std::vector<ComponentName> components);
    static Component* EntityToComponent(Entity entity,
                                        ComponentName name);

    static std::set<Entity>*                         getEntities();
    static UMap<ResourceName, Resource>*             getResources();
    static UMapVec<ComponentName, Component>*        getComponents();
    static std::set<SPtr<System>, CompareSharedPtr>* getSystems();

    static Entity NewEntity();
    static void AddComponent(Entity entity, ComponentName component_name,
                             SPtr<Component> component);
    static void AddSystem(SPtr<System> system);
    static void AddResource(SPtr<Resource> resource);

    static void RemoveEntity(Entity entity);
    static void RemoveComponent(Component component);
    static void RemoveSystem(SystemName name);
    static void RemoveResource(Resource resource);

private:
    static SetPtr<Entity>                       entities;
    static SetSPtr<System>                      systems;
    static UMapPtr<ResourceName, Resource>      resources;
    static UMapVecPtr<ComponentName, Component> components;
};

} // namespace ECS

#endif
