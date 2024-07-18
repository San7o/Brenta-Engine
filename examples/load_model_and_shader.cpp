#include <iostream>
#include <filesystem>

#include "gl_helper.h"
#include "screen.h"
#include "shader.h"
#include "model.h"

using namespace ECS;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const bool isMouseCaptured = false;

int main() {

    /* Initialize the screen */
    Screen::Init(SCR_WIDTH, SCR_HEIGHT);

    /* Load OpenGL */
    GL::LoadOpenGL((GLADloadproc)Screen::GetProcAddress(),
                    SCR_WIDTH, SCR_HEIGHT);

    /* Load the model */
    Model ourModel(std::filesystem::absolute("assets/models/backpack/backpack.obj"));

    /* Load the shader */
    Shader::NewShader("default_shader",
                      std::filesystem::absolute("game/shaders/shader.vs"),
                      std::filesystem::absolute("game/shaders/shader.fs"));


    while(!Screen::isWindowClosed()) {

        /* Input */
        if (Screen::isKeyPressed(GLFW_KEY_ESCAPE))
            Screen::SetClose();

        /* Clear */
        GL::SetColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::Clear();

        /* Draw */
        Shader::Use("default_shader");

        /* Make transformations */
        glm::mat4 view = glm::mat4(1.0f); /* Camera position */
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                           (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));

        Shader::SetMat4("default_shader", "view", view);
        Shader::SetMat4("default_shader", "projection", projection);
        Shader::SetMat4("default_shader", "model", model);

        /* Draw the model */
        ourModel.Draw("default_shader");

        Screen::PollEvents();
        Screen::SwapBuffers();
    }

    Screen::Terminate();
    return 0;
}
