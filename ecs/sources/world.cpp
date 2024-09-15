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

#include "world.hpp"

#include "engine_logger.hpp"
#include "engine_time.hpp"

#include <algorithm>

using namespace brenta::ecs;

SetPtr<entity_t> world::entities;
UMapPtr<std::type_index, resource> world::resources;
UMapVecPtr<std::type_index, component> world::components;

template <>
void world::query_components_rec<none>(std::vector<entity_t> *entities)
{
}

void world::init()
{
    using namespace types;
    world::entities = std::make_unique<std::set<entity_t>>();
    world::resources = std::make_unique<UMap<std::type_index, resource>>();
    world::components = std::make_unique<UMapVec<std::type_index, component>>();

    INFO("World initialized");
}

void world::destroy()
{
    world::entities.reset();
    world::components.reset();
    world::resources.reset();

    INFO("World deleted");
}

void world::tick()
{
    world::run_systems();
}

entity_t world::new_entity()
{
    if (!world::entities)
    {
        ERROR("Cannot create entity: world not initialized");
        return -1;
    }

    if (world::entities->empty())
    {
        world::entities->insert(1);
        return 1;
    }

    entity_t new_entity = *(world::entities->rbegin()) + 1;
    world::entities->insert(new_entity);

    INFO("New entity created: {}", new_entity);

    return new_entity;
}

std::set<entity_t> *world::get_entities()
{
    if (!world::entities)
    {
        ERROR("Cannot get entities: world not initialized");
        return nullptr;
    }
    return world::entities.get();
}

UMap<std::type_index, resource> *world::get_resources()
{
    if (!world::resources)
    {
        ERROR("Cannot get resources: world not initialized");
        return nullptr;
    }
    return world::resources.get();
}

UMapVec<std::type_index, component> *world::get_components()
{
    if (!world::components)
    {
        ERROR("Cannot get components: world not initialized");
        return nullptr;
    }
    return world::components.get();
}

void world::remove_entity(entity_t entity)
{
    if (!world::entities)
    {
        ERROR("Cannot remove entity: world not initialized");
        return;
    }

    world::entities->erase(entity);

    for (auto iter = world::components->begin();
         iter != world::components->end(); iter++)
    {
        iter->second.erase(
            std::remove_if(iter->second.begin(), iter->second.end(),
                           [&entity](const std::shared_ptr<component> &elem)
                           { return elem->entity == entity; }),
            iter->second.end());
    }

    INFO("Entity removed: {}", entity);
}
