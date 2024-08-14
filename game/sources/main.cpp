#include "engine.h"
#include "game_ecs.h"

#include <filesystem>

using namespace ECS;

// Default resolution
const int SCR_WIDTH = 1280; 
const int SCR_HEIGHT = 720;







// BEGIN TESTING ---------------------------------------------------

// Initialize particles
std::vector<glm::vec3> initialPositions(1000, glm::vec3(7.0f));  // 1000 particles
std::vector<glm::vec3> initialVelocities(1000);  // 1000 velocities
GLuint vao, vbo[2], fbo[2];
bool first = true;
int current = 0;

void checkOpenGLError(const std::string& functionName) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after " << functionName << ": " << error << std::endl;
    }
}

// Setup function for buffers and shaders
void setupParticles() {
    
    // Generate and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs for position and velocity
    glGenBuffers(2, vbo);

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

    // Create fbos
    glGenBuffers(2, fbo);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, fbo[0]);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, 1000 * 2 * sizeof(glm::vec3), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, fbo[1]);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, 1000 * 2 * sizeof(glm::vec3), NULL, GL_DYNAMIC_COPY);
    checkOpenGLError("glBindBufferBase");

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

    glBindBuffer(GL_ARRAY_BUFFER, fbo[!current]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(0);

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
    current = !current; // Swap buffers
}

// Render particles
void renderParticles() {
    Shader::Use("particle_render");
    glBindVertexArray(vao);

    glDrawArrays(GL_POINTS, 0, 1000);
    
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
    const GLchar* varyings[] = { "outPosition", "outVelocity" };
    Shader::NewVertexShader(
                    "particle_update",
                    std::filesystem::absolute("game/shaders/particle_update.vs"),
                    varyings,
                    2
                    );
    Shader::NewShader(
                    "particle_render",
                    std::filesystem::absolute("game/shaders/particle_render.vs"),
                    std::filesystem::absolute("game/shaders/particle_render.fs")
                    );
    setupParticles();
    // ----- TESTING -----

    while(!Screen::isWindowClosed()) {

        GL::SetColor(0.2f, 0.2f, 0.207f, 1.0f);
        GL::Clear();



        // ----- TESTING -----
        updateParticles(Time::GetDeltaTime());
        //renderParticles();

        // Get feedback
        glFlush();
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, fbo[current]);
        glm::vec3 feedback[3];
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
        std::cout << feedback[0].x << " "
                  << feedback[0].y << " "
                  << feedback[0].z << ", "
                  << feedback[1].x << " "
                  << feedback[1].y << " "
                  << feedback[1].z << ", "
                  << feedback[2].x << " "
                  << feedback[2].y << " "
                  << feedback[2].z << std::endl;
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


