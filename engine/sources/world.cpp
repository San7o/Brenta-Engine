#include "world.h"

#include <algorithm>

using namespace ECS;

SetPtr<Entity>                       World::entities;
SetSPtr<System>                      World::systems;
UMapPtr<ResourceName, Resource>      World::resources;
UMapVecPtr<ComponentName, Component> World::components;

void World::Init()
{
    using namespace Types;
    World::entities   = std::make_unique<std::set<Entity>>();
    World::systems    = std::make_unique<std::set<SPtr<System>,
                                         CompareSharedPtr>>();
    World::resources  = std::make_unique<UMap<ResourceName, Resource>>();
    World::components = std::make_unique<UMapVec<ComponentName, Component>>();
}

void World::Delete()
{
    World::entities.reset();
    World::components.reset();
    World::systems.reset();
    World::resources.reset();
}

void World::Tick()
{
    for (auto& system : *World::systems) {
        system->function();
    }
}

std::vector<Entity> World::QueryComponents(std::vector<ComponentName> components)
{
    // TODO
    return {};
}

std::set<Entity>* World::getEntities()
{
    return World::entities.get();
}

std::set<SPtr<System>, CompareSharedPtr>* World::getSystems()
{
    return World::systems.get();
}

UMap<ResourceName, Resource>* World::getResources()
{
    return World::resources.get();
}

UMapVec<ComponentName, Component>* World::getComponents()
{
    return World::components.get();
}

Types::Entity World::NewEntity()
{
    if(!World::entities) {
        return -1;
    }

    if(World::entities->empty()) {
        World::entities->insert(1);
        return 1;
        
    }
    else {
        Entity new_entity = *(World::entities->rbegin()) + 1;
        World::entities->insert(new_entity);
        return new_entity;
    }
}

void World::AddSystem(SPtr<System> system)
{
    if(!World::systems) {
        return;
    }

    World::systems->insert(system);
}

void World::AddResource(SPtr<Resource> resource)
{
    if(!World::resources) {
        return;
    }

    World::resources->insert({resource->name, resource});
}

void World::AddComponent(SPtr<Component> component)
{
    if(!World::components) {
        return;
    }

    if(!World::components->count((*component).name)) {
        World::components->insert({(*component).name,{component}});
    }
    else {
        World::components->at((*component).name).push_back(component);
    }
}

void World::RemoveEntity(Entity entity)
{
    if(!World::entities) {
        return;
    }

    World::entities->erase(entity);
}

void World::RemoveSystem(SystemName name)
{
    if(!World::systems) {
        return;
    }

    auto it = std::find_if(World::systems->begin(), World::systems->end(), 
                [&name](SPtr<System> s) {
                    return s->name == name;
                 });
    if (it != World::systems->end()) {
        World::systems->erase(it);
    }
}

void World::RemoveResource(Resource resource)
{
    if(!World::resources) {
        return;
    }

    World::resources->erase(resource.name);
}

void World::RemoveComponent(Component component)
{
    if(!World::components) {
        return;
    }

    if(World::components->count(component.name)) {
        auto& vec = World::components->at(component.name);
        auto it = std::find_if(vec.begin(), vec.end(), 
                        [&component](SPtr<Component> c) {
                            return c->entity == component.entity;
                        });
        if(it != vec.end()) {
            vec.erase(it);
        }
    }
}
