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

/* libc */
#include <stdio.h>           /* Standard I/O */
#include <cmath>             /* Math functions */


/* Function prototypes */
void framebuffer_size_callback(GLFWwindow* window, int width,
                               int height);  
void processInput(GLFWwindow *window);


/* Constants */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


/* Main function */
int main() {

    /* 
     * Setup window
     */

    /* Initialize GLFW */
    if (glfwInit() == GL_FALSE)
    {
        fprintf(stderr, "Failed to initialize GLFW on init\n");
        return -1;
    }

    /* Setup GLFW window properties */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* Use core profile */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window on create windw\n");
        glfwTerminate();
        return -1;
    }

    /* Make the window's context the current thread */
    glfwMakeContextCurrent(window);


    /* 
     * Load OpenGL 
     */

    /* Load all OpenGL function pointers */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD on GLLoader\n");
        return -1;
    }

    /* Set the viewport size */
    glViewport(0, 0, 800, 600);
    
    /* Callback function for resizing the window, updates the viewport */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 

    /* 
     * Create Shaders
     */
    
    Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs");

    /*
     * Create vertices
     *
     * General steps:
     * - Create vertex buffer object (VBO) to store vertices
     * - Create element buffer object (EBO) to store indices
     * - Create vertex array object (VAO) to store vertex attribute
     * - Bind VAO, VBO, EBO
     * - Copy vertices to VBO
     * - Copy indices to EBO
     * - Link vertex attributes to vertex shader
     * - Render loop
     */

    /* Vertices in normalized device coordinates */
    float vertices[] = {
         /* positions */     /* colors */       /* Texture coords */
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  /* top right    */
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  /* bottom right */
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  /* bottom left  */
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f   /* top left     */
    };
    /* Indices */
    unsigned int indices[] = {
        0, 1, 2,   /* first triangle */
        0, 3, 2   /* second triangle */
    };

    /* 
     * Vertex Buffer Object (VBO)
     *
     * Can store large number of vertices
     * so avoid sending data to the GPU for each vertex.
     *
     * Vertex Array Object (VAO)
     *
     * A vertex array object stores the following:
     * - Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
     * - Vertex attribute configurations via glVertexAttribPointer.
     * - VBOs associated with vertex attributes by calls to glVertexAttribPointer.
     *
     * EBO (Element Buffer Object)
     *
     * An element buffer object is a buffer that stores indices that OpenGL
     * uses to decide what vertices to draw.
     */
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /* bind the Vertex Array Object first,
     * then bind and set vertex buffer(s),
     * and then configure vertex attributes(s). */
    glBindVertexArray(VAO);

    /*
     * glBindBuffer(GLenum target, GLuint buffer);
     *
     * Binds a buffer object to a buffer target.
     *
     */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /*
     * glBufferData(GLenum target, GLsizeiptr size, const void* data,
     *             GLenum usage);
     *
     * Copy user defined data into the currently bound buffer.
     *
     * Usage can be:
     * - GL_STATIC_DRAW: The data will be modified once and used many times.
     * - GL_DYNAMIC_DRAW: The data will be modified repeatedly and used many times.
     * - GL_STREAM_DRAW: The data will be modified once and used once.
     *
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    /* 
     * Linking vertex attributes
     */

    /*
     * glVertexAttribPointer(GLuint index, GLint size, GLenum type,
     *                     GLboolean normalized, GLsizei stride,
     *                     const void* pointer);
     *
     * Specifies the location and data format of the vertex attributes.
     *
     * Parameters:
     * - index: Specifies the index of the vertex attribute.
     * - size: Specifies the number of components per vertex attribute.
     * - type: Specifies the data type of each component in the array.
     * - normalized: Specifies whether fixed-point data values should be normalized.
     * - stride: Specifies the byte offset between consecutive vertex attributes.
     * - pointer: Specifies a offset of the first component in the array.
     */
    /* position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* color attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    /* texture coord attribute */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /*
     * Texure
     */

    unsigned int texture1, texture2;
    /* First texture */
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    /* Texture Wrapping */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* Texture Filtering */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* Mipmaps: Generate a set of textures with different resolutions, OpenGL selects
     * the most appropriate based on the distance of the object. */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Read image */
    int width1, height1, nrChannels1;
    stbi_set_flip_vertically_on_load(true); /* Flip the image */
    unsigned char *data1 = stbi_load("assets/images/container.jpg",
                                     &width1, &height1, &nrChannels1, 0);
    if (data1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D); /* Automatically generate
                                            all the mipmap levels */
    }
    else
    {
        fprintf(stderr, "Failed to load texture\n");
    }
    stbi_image_free(data1);

    /* Second texture */
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width2, height2, nrChannels2;
    unsigned char *data2 = stbi_load("assets/images/awesomeface.png",
                                     &width2, &height2, &nrChannels2, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D); /* Automatically generate
                                            all the mipmap levels */
    }
    else
    {
        fprintf(stderr, "Failed to load texture\n");
    }
    stbi_image_free(data2);


    /* Set the texture uniform in the shader */
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


    /* uncomment this call to draw in wireframe polygons. */
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    /* 
     * Render loop
     */

    while(!glfwWindowShouldClose(window))
    {
        /* Input */
        processInput(window);


        /* Rendering commands here */

        /* Set the color of the window */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        /* Clear the color buffer and updates it to the set color */
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0); /* Activate the texture unit */
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
       
        /* Create transformations */
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        /* Run the shader */
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        /* check and call events and swap the buffers */

        /*
         * glfwSwapBuffers(GLFWwindow* window);
         *
         * Swaps the front and back buffers of the specified window.
         *
         * Why do we need double buffering?
         * When an application draws in a single buffer the resulting
         * image may display flickering issues. This is because the
         * resulting output image is not drawn in an instant, but drawn
         * pixel by pixel and usually from left to right and top to
         * bottom. Because this image is not displayed at an instant
         * to the user while still being rendered to, the result may
         * contain artifacts. To circumvent these issues, windowing
         * applications apply a double buffer for rendering. The front
         * buffer contains the final output image that is shown at
         * the screen, while all the rendering commands draw to the
         * back buffer. As soon as all the rendering commands are
         * finished we swap the back buffer to the front buffer so
         * the image can be displayed without still being rendered
         * to, removing all the aforementioned artifacts. 
         *
         */
        glfwSwapBuffers(window);

        /* Process all pending events */
        glfwPollEvents();
    }


    /* de-allocate all resources */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    /* Terminate GLFW */
    glfwTerminate();

    return 0;
}

/*
 * framebuffer_size_callback(GLFWwindow* window, int width,
 *                           int height);
 *
 * This function is called whenever the window is resized, and
 * updates the viewport size so that OpenGL renders correctly.
 *
 */
void framebuffer_size_callback(GLFWwindow* window, int width,
                               int height)
{
    glViewport(0, 0, width, height);
}

// TODO: Replace this with `glfwSetKeyCallback`
void processInput(GLFWwindow *window)
{
    /*
     * glfwGetKey(GLFWwindow* window, int key);
     *
     * This function returns whether the specified key is pressed.
     *
     */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
