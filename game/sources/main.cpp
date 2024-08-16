#include "engine.h"
#include "game_ecs.h"

#include <filesystem>
#include <bitset>

using namespace ECS;

// Default resolution
const int SCR_WIDTH = 1280; 
const int SCR_HEIGHT = 720;







// BEGIN TESTING ---------------------------------------------------

#define MAX_PARTICLES 1000

// Emitter
glm::vec3 emitterPosition(1.0f, 3.0f, 1.0f);

// Particles TODO: particle struct
std::vector<glm::vec3> initialPositions(MAX_PARTICLES, emitterPosition);  // 1000 particles
std::vector<glm::vec3> initialVelocities(MAX_PARTICLES, glm::vec3(0.0f, 3.0f, 0.0f));  // 1000 velocities
std::vector<float> initialTimeToLive(MAX_PARTICLES, 3.0);  // Time to live in seconds
GLuint vao, vbo[3], fbo[2];
bool first = true;
int current = 0;
GLuint written = 0;
int particle_texture_atlas;

void checkOpenGLError(const std::string& functionName) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after " << functionName << ": " << error << std::endl;
    }
}

// Setup function for buffers and shaders
void setupParticles() {
    
    // This is needed to render points
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Generate and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs for position and velocity
    glGenBuffers(3, vbo);

    // Initial positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, initialPositions.size() * sizeof(glm::vec3), &initialPositions[0], GL_DYNAMIC_COPY);
    checkOpenGLError("glBufferData");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    checkOpenGLError("glVertexAttribPointer");

    // Initial velocities
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, initialVelocities.size() * sizeof(glm::vec3), &initialVelocities[0], GL_DYNAMIC_COPY);
    checkOpenGLError("glBufferData");
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    checkOpenGLError("glVertexAttribPointer");

    // Initial time to live
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, initialTimeToLive.size() * sizeof(float), &initialTimeToLive[0], GL_DYNAMIC_COPY);
    checkOpenGLError("glBufferData");
    glVertexAttribIPointer(2, 1, GL_INT, GL_FALSE, (void*)0);
    glEnableVertexAttribArray(2);
    checkOpenGLError("glVertexAttribPointer");

    // Create fbos
    glGenBuffers(2, fbo);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, fbo[0]);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, MAX_PARTICLES * 2 * sizeof(glm::vec3) + MAX_PARTICLES * sizeof(float), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, fbo[1]);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, MAX_PARTICLES * 2 * sizeof(glm::vec3) + MAX_PARTICLES * sizeof(float), NULL, GL_DYNAMIC_COPY);
    checkOpenGLError("glBindBufferBase");

    // Load Texture Atlas
    particle_texture_atlas = ECS::Texture::LoadTexture(
        "particle_atlas.png",
        std::filesystem::absolute("assets/textures").string().c_str()
    );

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Update particles using Transform Feedback
// directly from the wiki
void updateParticles(float deltaTime) {

    Shader::Use("particle_update");
    Shader::SetFloat("particle_update", "deltaTime", deltaTime);
    checkOpenGLError("Shader::SetFloat");

    glBindVertexArray(vao);

    checkOpenGLError("glVertexAttribPointer");

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fbo[current]);
    checkOpenGLError("glBindBufferBase");
    
    /*
    GLuint query;
    glGenQueries(1, &query);
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
    */

    if (!first) { // MUST BE HERE
        glBindBuffer(GL_ARRAY_BUFFER, fbo[!current]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)(2 * sizeof(glm::vec3)));
        glEnableVertexAttribArray(2);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(2);
        first = false;
    }
    glEnableVertexAttribArray(0);

    // Start transform feedback
    glEnable(GL_RASTERIZER_DISCARD);  // Disable rasterization
    glBeginTransformFeedback(GL_POINTS); // Enter transform feedback mode
    checkOpenGLError("glBeginTransformFeedback");

    glDrawArrays(GL_POINTS, 0, 1000);
    checkOpenGLError("glDrawTransformFeedback");

    // End the query
    /*
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &written);
    glDeleteQueries(1, &query);
    std::cout << "Written: " << written << std::endl;
    */

    glEndTransformFeedback(); // Exit transform feedback mode
    glDisable(GL_RASTERIZER_DISCARD);  // Enable rasterization
    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
    current = !current; // Swap buffers
}

// Render particles
void renderParticles() {
    Shader::Use("particle_render");

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, fbo[current]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(float), (void*)(2 * sizeof(glm::vec3)));
    checkOpenGLError("glVertexAttribPointer");
    glEnableVertexAttribArray(2);

    // Set uniforms
    Types::Translation t = Types::Translation();
    t.setView(Camera::GetViewMatrix());
    t.setProjection(Camera::GetProjectionMatrix());
    t.setModel(glm::mat4(1.0f));
    t.setShader("particle_render");
    Shader::SetInt("particle_render", "atlas_width", 8);
    Shader::SetInt("particle_render", "atlas_height", 8);
    Shader::SetInt("particle_render", "atlas_index", 52);

    // Set Textures
    ECS::Texture::ActiveTexture(GL_TEXTURE0);
    ECS::Texture::BindTexture(GL_TEXTURE_2D, particle_texture_atlas);

    glDrawArrays(GL_POINTS, 0, 1000);
    checkOpenGLError("glDrawArrays");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


// END TESTING ---------------------------------------------------




int main() {

    Logger::Init();                              /* defauls to ./logs/log.txt */
    Logger::SetLogLevel(Types::LogLevel::DEBUG); /* default = WARNING */

    Screen::Init(SCR_WIDTH, SCR_HEIGHT);
    GL::LoadOpenGL();
    World::Init();

    InitPlayerEntity();
    //InitCubeEntity();
    InitFloorEntity();
    InitDirectionalLightEntity();
    InitPointLightEntity();
    InitSphereEntity();
    InitRobotEntity();

    InitDirectionalLightSystem();
    InitPointLightsSystem();
    InitRendererSystem();
    //InitFpsSystem();
    InitDebugTextSystem();
    InitPhysicsSystem();
    InitCollisionsSystem();

    InitToggleWireframeCallback();
    InitCloseWindowCallback();
    InitCameraMouseCallback();
    InitPlayGuitarCallback();
    
    InitWireframeResource();

    Audio::LoadAudio("guitar",
             std::filesystem::absolute("assets/audio/guitar.wav"));

    // ----- TESTING -----
    const GLchar* varyings[] = { "outPosition", "outVelocity", "outTTL" };
    Shader::New(
       varyings,
       3,
       "particle_update",
       GL_VERTEX_SHADER,
       std::filesystem::absolute("game/shaders/particle_update.vs")
       //GL_GEOMETRY_SHADER,
       //std::filesystem::absolute("game/shaders/particle_update.gs")
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
    setupParticles();
    // ----- TESTING -----

    while(!Screen::isWindowClosed()) {

        GL::SetColor(0.2f, 0.2f, 0.207f, 1.0f);
        GL::Clear();



        // ----- TESTING -----
        updateParticles(Time::GetDeltaTime());
        renderParticles();

        // Get feedback
        glFlush();
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, fbo[current]);
        glm::vec3 feedback[2];
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
        std::cout << feedback[0].x << " "
                  << feedback[0].y << " "
                  << feedback[0].z << ", "
                  << feedback[1].x << " "
                  << feedback[1].y << " "
                  << feedback[1].z << ", ";
        float ttl;
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 2 * sizeof(glm::vec3), sizeof(float), &ttl);
        std::cout << "TTL: " << ttl << std::endl;
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
        // ----- TESTING -----
        


        World::Tick();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }
    
    World::Delete();
    Screen::Terminate();
    Logger::Close();
    return 0;
}


