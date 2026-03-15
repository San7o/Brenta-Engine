// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/particles.hpp>
#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/particle_emitter_ecs_component.hpp>

#include <demo/entities.hpp>

using namespace viotecs;
using namespace brenta;

void init_particle_emitter_entity(tenno::shared_ptr<brenta::Camera> camera,
                                  tenno::weak_ptr<FrameBuffer> fb)
{
  auto emitter_builder =
    ParticleEmitter::Builder()
    .with_camera(camera)
    .starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
    .starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
    .starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
    .starting_time_to_live(0.5f)
    .num_particles(1000)
    .spawn_rate(0.99f)
    .scale(1.0f)
    .atlas_path("examples/assets/textures/particle_atlas.png")
    .atlas_width(8)
    .atlas_height(8)
    .atlas_index(0);

  auto emitter =
    World::new_entity()
    .add_component<ParticleEmitterEcsComponent>(emitter_builder, fb);

  return;
}
