#pragma once

#include "resource.hpp"

using namespace Brenta::ECS;

struct WireframeResource : Resource
{
    bool enabled;
    WireframeResource (bool e) : enabled (e)
    {
    }
};
