/*
 * This file contains the definition of the Component type.
 *
 * A Component is a piece of data that is attached to an Entity.
 * Components are used to store data that is associated with a game object.
 * For example, a Position component could store the position of a game object.
 */

#pragma once

#include "entity.h"

namespace ECS {

/**
 * Example Usage:
 * 
 * struct TransformComponent : Component {
 *   glm::vec3 position;
 *   glm::vec3 rotation;
 *   float scale;
 *
 *   TransformComponent() : ... 
 *   TransformComponent(glm::vec3 position, ...
 * };
 *
 * You need to provide a default constructor,
 * any other constructor is optional.
 */
struct Component {
    Entity entity;
    Component() {};

    bool operator==(const Component& other) const {
        return (entity == other.entity);
    }
};

} // namespace ECS
