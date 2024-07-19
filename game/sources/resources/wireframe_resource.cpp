#include "resources/wireframe_resource.h"
#include "world.h"

#include <memory>

using namespace ECS;

void InitWireframeResource() {
    auto wireframe_resource = std::make_shared<WireframeResource>(false);
    World::AddResource("WireframeResource", wireframe_resource);
}
