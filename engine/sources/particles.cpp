#include "particles.h"

#include <iostream>
#include <filesystem>
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "time.h"
#include "translation.h"

using namespace ECS;

ParticleEmitter::ParticleEmitter() {
    starting_position = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_spread   = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_timeToLive = 1.0f;
    num_particles = MAX_PARTICLES;
    spawn_rate = 0.01f;
    atlas = 0;
    atlas_width = 8;
    atlas_height = 8;
    atlas_index = 0;
    current = 0;
}

ParticleEmitter::ParticleEmitter(glm::vec3 starting_position,
                                 glm::vec3 starting_velocity,
                                 glm::vec3 starting_spread,
                                 float starting_timeToLive,
                                 int num_particles,
                                 float spawn_rate,
                                 std::string atlas_path,
                                 int atlas_width,
                                 int atlas_height,
                                 int atlas_index) {
    this->starting_position = starting_position;
    this->starting_velocity = starting_velocity;
    this->starting_spread = starting_spread;
    this->starting_timeToLive = starting_timeToLive;
    this->num_particles = num_particles;
    this->spawn_rate = spawn_rate;
    this->atlas = atlas;
    this->atlas_width = atlas_width;
    this->atlas_height = atlas_height;
    this->atlas_index = atlas_index;
    this->current = 0;

    // Load Texture Atlas
    this->atlas = ECS::Texture::LoadTexture(atlas_path);

    // Create shaders
    const GLchar* varyings[] = { "outPosition", "outVelocity", "outTTL" };
    Shader::New(
       varyings,
       3,
       "particle_update",
       GL_VERTEX_SHADER,
       std::filesystem::absolute("game/shaders/particle_update.vs")
    );
    Shader::New(
        "particle_render",
        GL_VERTEX_SHADER,
        std::filesystem::absolute("game/shaders/particle_render.vs").string(),
        GL_GEOMETRY_SHADER,
        std::filesystem::absolute("game/shaders/particle_render.gs").string(),
        GL_FRAGMENT_SHADER,
        std::filesystem::absolute("game/shaders/particle_render.fs").string()
    );
    
    // This is needed to render points
    glEnable(GL_PROGRAM_POINT_SIZE);

    this->vao.Init();
    this->vao.Bind();
    checkOpenGLError("vao bind");

    // Create fbos
    glGenBuffers(2, this->fbo);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, this->fbo[0]);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, MAX_PARTICLES * 2 * sizeof(glm::vec3) + MAX_PARTICLES * sizeof(float), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, this->fbo[1]);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, MAX_PARTICLES * 2 * sizeof(glm::vec3) + MAX_PARTICLES * sizeof(float), NULL, GL_DYNAMIC_COPY);
    checkOpenGLError("glBindBufferBase A");

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->vao.Unbind();
}

ParticleEmitter::~ParticleEmitter() {
    glDeleteBuffers(2, fbo);
}

// Update particles using Transform Feedback
// directly from the wiki
void ParticleEmitter::updateParticles(float deltaTime) {

    Shader::Use("particle_update");
    Shader::SetFloat("particle_update", "deltaTime", deltaTime);
    Shader::SetVec3("particle_update", "emitterPos", this->starting_position);
    Shader::SetVec3("particle_update", "emitterSpread", this->starting_spread);
    Shader::SetFloat("particle_update", "spawnProbability", this->spawn_rate);
    Shader::SetVec3("particle_update", "emitterVel", this->starting_velocity);
    Shader::SetFloat("particle_update", "emitterTTL", this->starting_timeToLive);
    checkOpenGLError("settin update shader");

    this->vao.Bind();

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fbo[current]);
    checkOpenGLError("glBindBufferBase B");

    glBindBuffer(GL_ARRAY_BUFFER, fbo[!current]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)(2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

    // Start transform feedback
    glEnable(GL_RASTERIZER_DISCARD);  // Disable rasterization
    glBeginTransformFeedback(GL_POINTS); // Enter transform feedback mode
    checkOpenGLError("glBeginTransformFeedback");

    glDrawArrays(GL_POINTS, 0, 1000);
    checkOpenGLError("glDrawTransformFeedback");

    glEndTransformFeedback(); // Exit transform feedback mode
    glDisable(GL_RASTERIZER_DISCARD);  // Enable rasterization
    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
    this->vao.Unbind();
    current = !current; // Swap buffers
}

// Render particles
void ParticleEmitter::renderParticles() {
    Shader::Use("particle_render");

    this->vao.Bind();

    glBindBuffer(GL_ARRAY_BUFFER, fbo[current]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(float), (void*)(2 * sizeof(glm::vec3)));
    checkOpenGLError("glVertexAttribPointer");
    glEnableVertexAttribArray(1);

    // Set uniforms
    Types::Translation t = Types::Translation();
    t.setView(Camera::GetViewMatrix());
    t.setProjection(Camera::GetProjectionMatrix());
    t.setModel(glm::mat4(1.0f));
    t.setShader("particle_render");
    Shader::SetInt("particle_render", "atlas_width", this->atlas_width);
    Shader::SetInt("particle_render", "atlas_height", this->atlas_height);
    Shader::SetInt("particle_render", "atlas_index", this->atlas_index);

    // Set Textures
    ECS::Texture::ActiveTexture(GL_TEXTURE0);
    ECS::Texture::BindTexture(GL_TEXTURE_2D, this->atlas);

    glDrawArrays(GL_POINTS, 0, 1000);
    checkOpenGLError("glDrawArrays");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    vao.Unbind();
}

void ParticleEmitter::checkOpenGLError(const std::string& functionName) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after " << functionName << ": " << error << std::endl;
    }
}
