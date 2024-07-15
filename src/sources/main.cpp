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
    vao.SetVertexData(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    /* normal data */
    vao.SetVertexData(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
                      (void*)(3*sizeof(float)));

    /* Create Camera */
    camera = Camera();

    /* Lighting */
    Shader lightSourceShader("src/shaders/light_source.vs", "src/shaders/light_source.fs");
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
    VAO light_vao;
    vbo.Bind();
    vao.SetVertexData(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    
    /* Render Loop */
    while(!display.isWindowClosed())
    {
        gtime.Update(display.GetTime());
        processInput();

        GL::SetColor(0.1f, 0.1f, 0.1f, 1.0f);
        GL::Clear();

        /* Run the shader */
        ourShader.use();
        ourShader.setVec3("objectColor", objectColor.x, objectColor.y, objectColor.z);
        ourShader.setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
        ourShader.setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
        ourShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
        ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        ourShader.setFloat("material.shininess", 32.0f);
        ourShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        /* Create transformations (view, projection, model) */
        Transformations trans;
        trans.view = camera.GetViewMatrix();
        trans.projection = glm::perspective(glm::radians(camera.Zoom),
                           (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("view", trans.view);
        ourShader.setMat4("projection", trans.projection);
        trans.model = glm::mat4(1.0f);
        ourShader.setMat4("model", trans.model);

        /* render the cube */
        vao.Bind();
        GL::DrawArrays(GL_TRIANGLES, 0, 36);

        /* light cube */
        lightSourceShader.use();
        lightSourceShader.setMat4("view", trans.view);
        lightSourceShader.setMat4("projection", trans.projection);
        trans.model = glm::mat4(1.0f);
        trans.model = glm::translate(trans.model, lightPos);
        trans.model = glm::scale(trans.model, glm::vec3(0.2f));
        lightSourceShader.setMat4("model", trans.model);
        light_vao.Bind();
        GL::DrawArrays(GL_TRIANGLES, 0, 36);

        /* check and call events and swap the buffers */
        display.SwapBuffers();
        display.PollEvents();
    }

    /* de-allocate all resources */
    vao.Delete();
    vbo.Delete();
    light_vao.Delete();
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
