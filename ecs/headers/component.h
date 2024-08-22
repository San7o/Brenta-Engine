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
