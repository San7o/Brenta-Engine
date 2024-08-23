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

using namespace Brenta::ECS;
using namespace Brenta::Utils;

SetPtr<Entity> World::entities;
UMapPtr<std::type_index, Resource> World::resources;
UMapVecPtr<std::type_index, Component> World::components;

template <> void World::QueryComponentsRec<None> (std::vector<Entity> *entities)
{
}

void World::Init ()
{
    using namespace Types;
    World::entities = std::make_unique<std::set<Entity>> ();
    World::resources = std::make_unique<UMap<std::type_index, Resource>> ();
    World::components =
        std::make_unique<UMapVec<std::type_index, Component>> ();

    INFO ("World initialized");
}

void World::Delete ()
{
    World::entities.reset ();
    World::components.reset ();
    World::resources.reset ();

    INFO ("World deleted");
}

void World::Tick ()
{
    World::RunSystems ();
}

Entity World::NewEntity ()
{
    if (!World::entities)
    {
        ERROR ("Cannot create entity: world not initialized");
        return -1;
    }

    if (World::entities->empty ())
    {
        World::entities->insert (1);
        return 1;
    }

    Entity new_entity = *(World::entities->rbegin ()) + 1;
    World::entities->insert (new_entity);

    INFO ("New entity created: ", new_entity);

    return new_entity;
}

std::set<Entity> *World::getEntities ()
{
    if (!World::entities)
    {
        ERROR ("Cannot get entities: world not initialized");
        return nullptr;
    }
    return World::entities.get ();
}

UMap<std::type_index, Resource> *World::getResources ()
{
    if (!World::resources)
    {
        ERROR ("Cannot get resources: world not initialized");
        return nullptr;
    }
    return World::resources.get ();
}

UMapVec<std::type_index, Component> *World::getComponents ()
{
    if (!World::components)
    {
        ERROR ("Cannot get components: world not initialized");
        return nullptr;
    }
    return World::components.get ();
}

void World::RemoveEntity (Entity entity)
{
    if (!World::entities)
    {
        ERROR ("Cannot remove entity: world not initialized");
        return;
    }

    World::entities->erase (entity);

    for (auto iter = World::components->begin ();
         iter != World::components->end (); iter++)
    {
        iter->second.erase (
            std::remove_if (iter->second.begin (), iter->second.end (),
                            [&entity] (const std::shared_ptr<Component> &elem)
                            { return elem->entity == entity; }),
            iter->second.end ());
    }

    INFO ("Entity removed: ", entity);
}
