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

#pragma once

#include "entity.hpp"

namespace brenta
{

namespace ecs
{

/**
 * @brief Component class
 *
 * This class is used to create components. Components are used to store
 * data that is associated with an entity. For example, a Position
 * component could store the position of the entity.
 *
 * Example creating a component:
 *
 * ```
 * struct transform_component : component {
 *   glm::vec3 position;
 *   glm::vec3 rotation;
 *   float scale;
 *
 *   transform_component() : ...
 *   transform_component(glm::vec3 position, ...
 * };
 * ```
 *
 * You need to provide a default constructor,
 * any other constructor is optional.
 */
struct component
{
    entity_t entity;

    bool operator==(const component &other) const
    {
        return (entity == other.entity);
    }
};

} // namespace ecs

} // namespace brenta
