/*
 *
 *   Created on: 2024-07-02
 *   Author: Giovanni Santini
 *   License: MIT
 *
 */

/* Graphics Libraries */
#include <glad/glad.h>       /* OpenGL driver */
#include <GLFW/glfw3.h>      /* OpenGL windowing library */
#include <stb_image.h>       /* Image loading library */
/* Math Libraries */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* User-defined headers */
#include "shader.h"          /* Shader class */
#include "camera.h"          /* Camera class */
#include "display.h"         /* Display class */
#include "texture.h"         /* Texture class */
#include "gl.h"              /* OpenGL functions */
#include "vbo.h"             /* Vertex Buffer Object */
#include "vao.h"             /* Vertex Array Object */
#include "ebo.h"             /* Element Buffer Object */
#include "vertices.h"        /* Vertices */
/* libc */
#include <stdio.h>           /* Standard I/O */
#include <cmath>             /* Math functions */

/* Function prototypes */
void framebuffer_size_callback(GLFWwindow* window, int width,
                               int height);  
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void processInput();

/* Structs */
struct GameTime
{
    float currentTime = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    void Update(float newTime)
    {
        currentTime = newTime;
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
    }
} gtime;

struct Transformations {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    Transformations() :
        model(glm::mat4(1.0f)),
        view(glm::mat4(1.0f)),
        projection(glm::mat4(1.0f)) {}
};

/* Constants */
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

/* Global variables */
Display display;
Camera camera;

/* Main function */
int main() {

    /* Window */
    display = Display(SCR_WIDTH, SCR_HEIGHT);
    display.SetSizeCallback(framebuffer_size_callback);
    display.SetMouseCallback(mouseCallback);

    /* Load OpenGL */
    GL::LoadOpenGL((GLADloadproc)display.GetProcAddress(),
                    SCR_WIDTH, SCR_HEIGHT);

    /* Create shader */
    Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs");

    /* Copy Vertices (they are defined in vertices.h) */
    VAO vao;
    VBO vbo;
    vbo.CopyVertices(sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /* position attribute */
    vao.SetVertexData(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    /* texture coord attribute */
    vao.SetVertexData(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
                      (void*)(3*sizeof(float)));

    /* Load textures */
    unsigned int texture1, texture2;
    texture1 = Texture::LoadTexture("assets/images/container.jpg", GL_RGB);
    texture2 = Texture::LoadTexture("assets/images/awesomeface.png", GL_RGBA);

    /* Create Camera */
    camera = Camera();

    /* Set the texture uniform in the shader */
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    /* Render Loop */
    while(!display.isWindowClosed())
    {
        gtime.Update(display.GetTime());
        processInput();

        GL::SetColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::Clear();

        Texture::ActiveTexture(GL_TEXTURE0);
        Texture::BindTexture(GL_TEXTURE_2D, texture1);
        Texture::ActiveTexture(GL_TEXTURE1);
        Texture::BindTexture(GL_TEXTURE_2D, texture2);

        /* Run the shader */
        ourShader.use();

        /* Create transformations (view, projection, model) */
        Transformations trans;

        trans.view = camera.GetViewMatrix();
        trans.projection = glm::perspective(glm::radians(camera.Zoom),
                           (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.setMat4("view", trans.view);
        ourShader.setMat4("projection", trans.projection);
        
        /* Render */
        vao.Bind();
        for(unsigned int i = 0; i < 10; i++)
        {
            trans.model = glm::mat4(1.0f);
            trans.model = glm::translate(trans.model, cubePositions[i]);
            float angle = 20.0 * i;
            trans.model = glm::rotate(trans.model, glm::radians(angle),
                            glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", trans.model);
            GL::DrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* check and call events and swap the buffers */
        display.SwapBuffers();
        display.PollEvents();
    }

    /* de-allocate all resources */
    vao.Delete();
    vbo.Delete();
    display.Terminate();
    return 0;
}


void processInput()
{
    /* Close the window */
    if (display.isKeyPressed(GLFW_KEY_ESCAPE))
        display.SetClose();

    /* Change polygon mode */
    if (display.isKeyPressed(GLFW_KEY_C))
        GL::SetPoligonMode(true);
    if (display.isKeyPressed(GLFW_KEY_X))
        GL::SetPoligonMode(false);

    /* show cursor */ 
    if (display.isKeyPressed(GLFW_KEY_Q))
        display.SetMouseCapture(false);
    if (display.isKeyPressed(GLFW_KEY_E))
        display.SetMouseCapture(true);

    /* Camera zoom */
    if (display.isKeyPressed(GLFW_KEY_KP_ADD))
        camera.ProcessMouseScroll(30.0f * gtime.deltaTime);
    if (display.isKeyPressed(GLFW_KEY_KP_SUBTRACT))
        camera.ProcessMouseScroll(-10.0f * gtime.deltaTime);
   
    /* Camera movement */
    if (display.isKeyPressed(GLFW_KEY_UP))
        camera.ProcessKeyboard(Camera_Movement::FORWARD, gtime.deltaTime);
    if (display.isKeyPressed(GLFW_KEY_DOWN))
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, gtime.deltaTime);
    if (display.isKeyPressed(GLFW_KEY_LEFT))
        camera.ProcessKeyboard(Camera_Movement::LEFT, gtime.deltaTime);
    if (display.isKeyPressed(GLFW_KEY_RIGHT))
        camera.ProcessKeyboard(Camera_Movement::RIGHT, gtime.deltaTime);

}

void framebuffer_size_callback(GLFWwindow* window, int width,
                               int height)
{
    GL::SetViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos, true);
}
