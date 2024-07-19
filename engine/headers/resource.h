/*
 * This file contains the definition of the Resource type.
 *
 * A Resource is a piece of global data that is used by the game.
 * It is saved in the World and can be accessed by any System.
 */

#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

namespace ECS {

namespace Types {

typedef std::string ResourceName;

struct Resource
{
    ResourceName name;
    Resource(ResourceName n) : name(n) {}
};

} // namespace Types

} // namespace ECS

#endif
