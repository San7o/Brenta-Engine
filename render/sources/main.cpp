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
#include <assimp/Importer.hpp>
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
#include "mesh.h"            /* Mesh class */
#include "model.h"           /* Model class */
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
    Shader ourShader("render/shaders/shader.vs", "render/shaders/shader.fs");

    /* Copy Vertices (they are defined in vertices.h) */
    VAO vao;
    VBO vbo;
    vbo.CopyVertices(sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /* position attribute */
    vao.SetVertexData(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    /* normal data */
    vao.SetVertexData(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                      (void*)(3*sizeof(float)));
    vao.SetVertexData(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                      (void*)(6*sizeof(float)));

    /* Create Camera */
    camera = Camera();

    /* Lighting */
    Shader lightSourceShader("render/shaders/light_source.vs", "render/shaders/light_source.fs");
    VAO light_vao;
    vbo.Bind();
    vao.SetVertexData(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glm::vec3 pointLightPositions[] = {
	    glm::vec3( 0.7f,  0.2f,  2.0f),
	    glm::vec3( 2.3f, -3.3f, -4.0f),
	    glm::vec3(-4.0f,  2.0f, -12.0f),
	    glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    /* Load model */
    Model ourModel("assets/models/backpack/backpack.obj");

    /* Load texture */
    unsigned int texture1 = Texture::LoadTexture("container2.png", "assets/textures");
    unsigned int texture2 = Texture::LoadTexture("container2_specular.png", "assets/textures");


    /* Render Loop */
    while(!display.isWindowClosed())
    {
        gtime.Update(display.GetTime());
        processInput();

        GL::SetColor(0.1f, 0.1f, 0.1f, 1.0f);
        GL::Clear();

        /* Texture */
        Texture::ActiveTexture(GL_TEXTURE0);
        Texture::BindTexture(GL_TEXTURE_2D, texture1);
        Texture::ActiveTexture(GL_TEXTURE1);
        Texture::BindTexture(GL_TEXTURE_2D, texture2);

        /* Run the shader */
        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

        ourShader.setInt("material.texture_diffuse1", 0);
        ourShader.setInt("material.texture_specular1", 1);
        ourShader.setFloat("material.shininess", 32.0f);
       
        /* Directional light */
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        ourShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

        /* Point light */
        ourShader.setVec3("pointLights[0].position", pointLightPositions[0].x,
                          pointLightPositions[0].y, pointLightPositions[0].z);
        ourShader.setVec3("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
        ourShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[0].constant", 1.0f);
        ourShader.setFloat("pointLights[0].linear", 0.09f);
        ourShader.setFloat("pointLights[0].quadratic", 0.032f);

        ourShader.setVec3("pointLights[1].position", pointLightPositions[1].x,
                          pointLightPositions[1].y, pointLightPositions[1].z);
        ourShader.setVec3("pointLights[1].ambient", 0.2f, 0.2f, 0.2f);
        ourShader.setVec3("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[1].constant", 1.0f);
        ourShader.setFloat("pointLights[1].linear", 0.09f);
        ourShader.setFloat("pointLights[1].quadratic", 0.032f);

        ourShader.setVec3("pointLights[2].position", pointLightPositions[2].x,
                          pointLightPositions[2].y, pointLightPositions[2].z);
        ourShader.setVec3("pointLights[2].ambient", 0.2f, 0.2f, 0.2f);
        ourShader.setVec3("pointLights[2].diffuse", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[2].constant", 1.0f);
        ourShader.setFloat("pointLights[2].linear", 0.09f);
        ourShader.setFloat("pointLights[2].quadratic", 0.032f);

        ourShader.setVec3("pointLights[3].position", pointLightPositions[3].x,
                          pointLightPositions[3].y, pointLightPositions[3].z);
        ourShader.setVec3("pointLights[3].ambient", 0.2f, 0.2f, 0.2f);
        ourShader.setVec3("pointLights[3].diffuse", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[3].constant", 1.0f);
        ourShader.setFloat("pointLights[3].linear", 0.09f);
        ourShader.setFloat("pointLights[3].quadratic", 0.032f);

        /* Create transformations (view, projection, model) */
        Transformations trans;
        trans.view = camera.GetViewMatrix();
        trans.projection = glm::perspective(glm::radians(camera.Zoom),
                           (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("view", trans.view);
        ourShader.setMat4("projection", trans.projection);

        /* Draw cubes */
        vao.Bind();
        for (unsigned int i = 0; i < 10; i++)
        {
            trans.model = glm::mat4(1.0f);
            trans.model = glm::translate(trans.model, cubePositions[i]);
            float angle = 20.0f * i;
            trans.model = glm::rotate(trans.model, glm::radians(angle),
                                      glm::vec3(1.0f, 0.3f, 0.5f));
            lightSourceShader.setMat4("model", trans.model);

            GL::DrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* Draw model */
        trans.model = glm::mat4(1.0f);
        trans.model = glm::translate(trans.model, glm::vec3(0.0f, 0.0f, -10.0f));
        lightSourceShader.setMat4("model", trans.model);
        ourModel.Draw(ourShader);

        /* light cube */
        lightSourceShader.use();
        lightSourceShader.setMat4("view", trans.view);
        lightSourceShader.setMat4("projection", trans.projection);
        for (unsigned int i = 0; i < 4; i++)
        {
           trans.model = glm::mat4(1.0f);
           trans.model = glm::translate(trans.model, pointLightPositions[i]);
           trans.model = glm::scale(trans.model, glm::vec3(0.2f));
           lightSourceShader.setMat4("model", trans.model);
           light_vao.Bind();
           GL::DrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* check and call events and swap the buffers */
        display.SwapBuffers();
        display.PollEvents();
    }

    /* de-allocate all resources */
    //vao.Delete();
    //vbo.Delete();
    //light_vao.Delete();
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
