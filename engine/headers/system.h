/*
 * This file contains the definition of the System type.
 *
 * A System is a function that processes game objects and are
 * called at any tick by the World. Systems functions are of the form:
 *
 * void MySystem();
 */
#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <functional>

namespace ECS {

namespace Types {

typedef std::string SystemName;

struct System {
    SystemName name;
    std::function<void()> function;

    System(SystemName n, std::function<void()> f)
            : name(n), function(f) {}
};

} // namespace Types

} // namespace ECS

#endif
