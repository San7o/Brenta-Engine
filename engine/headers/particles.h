#pragma once

#define MAX_PARTICLES 1000

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>       /* OpenGL driver */
#include <string>
#include <vector>

#include "vao.h"

namespace ECS
{

class ParticleEmitter
{
public:
    /* Starting position, velocity and time to live
     * of a new particle */
    glm::vec3 starting_position;
    glm::vec3 starting_velocity;
    /* Determines how spread apart are the particles */
    glm::vec3 starting_spread;
    /* Time to live of a new particle mesured in seconds */
    float starting_timeToLive;
    /* This number must be less then MAX_PARTICLES */
    int num_particles;
    /* The rate at which particles are spawned */
    float spawn_rate;
    /* Framebuffer objects, used to save the state
     * after the update phase */
    GLuint fbo[2];
    /* Used to mark the current fbo for reading */
    int current;
    int atlas;
    int atlas_width;
    int atlas_height;
    /* Index of the texture atlas to draw */
    int atlas_index;
    Types::VAO vao;

    ParticleEmitter();
    ParticleEmitter(glm::vec3 starting_position,
                    glm::vec3 starting_velocity,
                    glm::vec3 starting_spread,
                    float starting_timeToLive,
                    int num_particles,
                    float spawn_rate,
                    std::string atlas_path,
                    int atlas_width,
                    int atlas_height,
                    int atlas_index);
    ~ParticleEmitter();


    // Update particles using Transform Feedback
    // directly from the wiki
    void updateParticles(float deltaTime);

    // Render particles
    void renderParticles();

private:
    void checkOpenGLError(const std::string& functionName);
};

} // namespace ECS
