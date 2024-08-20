/*
 * This file contains the definition of the System type.
 *
 * A System is a function that processes game objects and are
 * called at any tick by the World. Systems functions are of the form:
 *
 * void MySystem();
 */

#pragma once

#include <string>
#include <functional>

namespace ECS {

namespace Types {

struct System {
    std::function<void()> function;
    System(std::function<void()> f) : function(f) {}
};

} // namespace Types

} // namespace ECS
