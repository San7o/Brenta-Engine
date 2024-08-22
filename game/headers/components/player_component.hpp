#pragma once

#include "engine.hpp"
#include "ecs.hpp"
#include "components/model_component.hpp"

#include <iostream>

using namespace ECS;
using namespace ECS::Types;

/* Player Component */
struct PlayerComponent : Component {
    PlayerComponent() {}
};
