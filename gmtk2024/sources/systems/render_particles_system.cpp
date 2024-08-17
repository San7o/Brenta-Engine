#include "systems/render_particles_system.h"
#include "components/particle_component.h"
#include "engine.h"

using namespace ECS;
using namespace ECS::Types;

void InitRenderParticleSystem() {
    auto render_particles_system = std::make_shared<System>("RenderParticlesSystem", []() {

        auto matches = World::QueryComponents(
                        {"ParticleComponent"});
        if (matches.empty()) return;

        for (auto match : matches) {

            auto particle_component = static_cast<ParticleComponent*>
                 (World::EntityToComponent(match, "ParticleComponent"));
            
            particle_component->emitter->updateParticles(Time::GetDeltaTime());
            particle_component->emitter->renderParticles();
        }
    });

    World::AddSystem(render_particles_system);
}
