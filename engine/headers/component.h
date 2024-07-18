/*
 * This file contains the definition of the Component type.
 *
 * A Component is a piece of data that is attached to an Entity.
 * Components are used to store data that is associated with a game object.
 * For example, a Position component could store the position of a game object.
 */

#ifndef COMPONENT_H
#define COMPONENT_H

#include "entity.h"

#include <string>

namespace ECS {

namespace Types {

typedef std::string ComponentName;

struct Component {
    Entity entity;
    ComponentName name;

    bool operator==(const Component& other) const {
        return (entity == other.entity && name == other.name);
    }
};

} // namespace Types

} // namespace ECS

#endif
