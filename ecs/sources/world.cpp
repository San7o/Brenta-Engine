#include "world.h"

#include <algorithm>
#include "engine_logger.h"
#include "engine_time.h"
#include "screen.h"

using namespace ECS;

SetPtr<Entity>    World::entities;
VecSPtr<System>   World::systems;
UMapPtr<std::type_index, Resource>     World::resources;
UMapVecPtr<std::type_index, Component> World::components;

template <>
void World::QueryComponentsRec<Types::None>(std::vector<Entity>* entities) {}

void World::Init()
{
    using namespace Types;
    World::entities   = std::make_unique<std::set<Entity>>();
    World::systems    = std::make_unique<std::vector<SPtr<System>>>();
    World::resources  = std::make_unique<UMap<std::type_index, Resource>>();
    World::components = std::make_unique<UMapVec<std::type_index, Component>>();

    Logger::Log(LogLevel::INFO, "World initialized");
}

void World::Delete()
{
    World::entities.reset();
    World::components.reset();
    World::systems.reset();
    World::resources.reset();

    Logger::Log(LogLevel::INFO, "World deleted");
}

void World::Tick()
{
    if (!World::systems) {
        Logger::Log(LogLevel::ERROR, "Cannot tick: world not initialized");
        return;
    }

    Time::Update(Screen::GetTime());
    for (auto& system : *World::systems) {
        system->function();
    }
}

Types::Entity World::NewEntity()
{
    if(!World::entities) {
        Logger::Log(LogLevel::ERROR, "Cannot create entity: world not initialized");
        return -1;
    }

    if(World::entities->empty()) {
        World::entities->insert(1);
        return 1;
        
    }

    Entity new_entity = *(World::entities->rbegin()) + 1;
    World::entities->insert(new_entity);

    Logger::Log(LogLevel::INFO,
                    "New entity created: " + std::to_string(new_entity));

    return new_entity;
}

std::set<Entity>* World::getEntities()
{
    if (!World::entities) {
        Logger::Log(LogLevel::ERROR, "Cannot get entities: world not initialized");
        return nullptr;
    }
    return World::entities.get();
}

std::vector<SPtr<System>>* World::getSystems()
{
    if (!World::systems) {
        Logger::Log(LogLevel::ERROR, "Cannot get systems: world not initialized");
        return nullptr;
    }
    return World::systems.get();
}

UMap<std::type_index, Resource>* World::getResources()
{
    if (!World::resources) {
        Logger::Log(LogLevel::ERROR, "Cannot get resources: world not initialized");
        return nullptr;
    }
    return World::resources.get();
}

UMapVec<std::type_index, Component>* World::getComponents()
{
    if (!World::components) {
        Logger::Log(LogLevel::ERROR, "Cannot get components: world not initialized");
        return nullptr;
    }
    return World::components.get();
}

void World::AddSystem(SPtr<System> system)
{
    if (!World::systems) {
        Logger::Log(LogLevel::ERROR, "Cannot add system: world not initialized");
        return;
    }

    World::systems->push_back(system);
    Logger::Log(LogLevel::INFO, "Added System");
}

void World::RemoveEntity(Entity entity)
{
    if(!World::entities) {
        Logger::Log(LogLevel::ERROR, "Cannot remove entity: world not initialized");
        return;
    }

    World::entities->erase(entity);

    for(auto iter = World::components->begin(); iter != World::components->end(); iter++) {
        iter->second.erase(
                std::remove_if(
                        iter->second.begin(),
                        iter->second.end(),
                        [&entity](const std::shared_ptr<Component>& elem) {
                            return elem->entity == entity;
                        }),
                iter->second.end()
        );
    }

    Logger::Log(LogLevel::INFO, "Entity removed: " + std::to_string(entity));
}
