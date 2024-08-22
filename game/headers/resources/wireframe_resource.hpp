#pragma once

#include "resource.hpp"

using namespace ECS;

struct WireframeResource : Resource {
    bool enabled;
    WireframeResource(bool e) : enabled(e) {}
};
