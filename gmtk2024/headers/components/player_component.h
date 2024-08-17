#pragma once

#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "engine.h"
#include "components/model_component.h"

#include <iostream>

using namespace ECS;
using namespace ECS::Types;

/* Player Component */
struct PlayerComponent : Component {
    float dashTreshold;
    float dashCharge;

    PlayerComponent(float dashTreshold, float dashCharge = 0.0f)
            : dashTreshold(dashTreshold), dashCharge(dashCharge) {}
};

#endif
