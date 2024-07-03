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

/* Vertex shaders */
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";


/* Fragment shaders */
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\0";


/* Main function */
int main() {

    /* Setup window */

    /* 
     *  glfwInit()
     *
     *  Initializes the GLFW library.
     *  Returns GL_TRUE if successful, GL_FALSE otherwise.
     *
     */
    if (glfwInit() == GL_FALSE)
    {
        fprintf(stderr, "Failed to initialize GLFW on init\n");
        return -1;
    }

    /*
     *  glfwWindowHint(int target, int hint);
     *
     *  This function sets hints for the next call
     *  to `glfwCreateWindow`.
     *  Arguments:
     *  - target: The target window hint to set.
     *  - hint: The new value of the window hint.
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* Use core profile */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /*
     * glfwCreateWindow(int width, int height, const char* title,
     *                  GLFWmonitor* monitor, GLFWwindow* share);
     *
     * This function creates a window and its associated OpenGL
     * context.
     *
     * Parameters:
     * - width:   The desired width, in screen coordinates.
     * - height:  The desired height, in screen coordinates.
     * - title:   The initial, UTF-8 encoded window title.
     * - monitor: The monitor to use for full screen mode,
     *            or NULL for windowed mode.
     * - share:   The window whose context to share resources
     *            with, or NULL to not share resources.
     *
     */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window on create windw\n");
        /*
         * glfwTerminate()
         *
         * This function destroys all remaining windows and
         *
         */
        glfwTerminate();
        return -1;
    }

    /*
     * glfwMakeContextCurrent(GLFWwindow* window);
     *
     *  This function makes the OpenGL or OpenGL ES context
     *  of the specified window current on the calling thread.
     *
     *  Parameters:
     *  - window: The window whose context to make current.
     *
     */
    glfwMakeContextCurrent(window);


    /* 
     * Load OpenGL 
     */

    /*
     * gladLoadGLLoader(GLADloadproc loader);
     *
     * This function loads the OpenGL functions.
     *
     * Parameters:
     * - loader: The address of the OpenGL function loader.
     *
     * Returns:
     * - GL_TRUE if successful, GL_FALSE otherwise.
     */
    // glfGetProcAddress() returns the address of the OpenGL function
    // based on the platform.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD on GLLoader\n");
        return -1;
    }

    /*
     * glViewport(int x, int y, int width, int height);
     *
     * Sets the size of the rendering window for OpenGL.
     *
     */
    glViewport(0, 0, 800, 600);
    
    /*
     * glfwSetFramebufferSizeCallback(GLFWwindow* window,
     *                               GLFWframebuffersizefun callback);
     *
     * Call the callback function when the window is resized.
     *
     */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 

    /* 
     * Compile vertex shader
     */
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    /* 
     * glShaderSource(GLuint shader, GLsizei count, const GLchar** string,
     *                const GLint* length);
     */
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    /*
     * Compile fragment shader
     */
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    /*
     * Link shaders
     */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
 
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    /* Delete shaders, once linked we don't need them anymore */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
         /* positions */     /* colors */
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  /* top right    */
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  /* bottom right */
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  /* bottom left  */
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,  /* top left     */
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   /* top center   */
    };
    unsigned int indices[] = {
        1, 2, 4,   /* first triangle */
        // 1, 2, 3   /* second triangle */
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* color attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* uncomment this call to draw in wireframe polygons. */
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* 
     * Render loop
     */

    /*
     * glfwWindowShouldClose(GLFWwindow* window);
     *
     * This function returns the value of the close flag of the
     * specified window.
     *
     */
    while(!glfwWindowShouldClose(window))
    {
        /* Input */
        processInput(window);


        /* Rendering commands here */

        /*
         * glClearColor(float red, float green, float blue, float alpha);
         *
         * Sets the color used when clearing the color buffer.
         *
         */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        /*
         * glClear(GLbitfield mask);
         *
         * Clears the specified buffers. The entire buffer
         * will be filled with the color set by `glClearColor`.
         * Possible values for mask:
         * - GL_COLOR_BUFFER_BIT: Color buffer
         * - GL_DEPTH_BUFFER_BIT: Depth buffer
         * - GL_STENCIL_BUFFER_BIT: Stencil buffer
         *
         */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Activate the shader */
        glUseProgram(shaderProgram);
   


        glBindVertexArray(VAO);
        /*
         * glDrawArrays(GLenum mode, GLint first, GLsizei count);
         *
         * Renders primitives from array data.
         */
         /* glDrawArrays(GL_TRIANGLES, 0, 3);*/

        /*
         * glDrawElements(GLenum mode, GLsizei count, GLenum type,
         *               const void* indices);
         *
         * Renders primitives from array data.
         */
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

        /*
         * glfwPollEvents();
         *
         * Processes all pending events, updates the window state.
         *
         */
        glfwPollEvents();
    }


    /* de-allocate all resources */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    /*
     * glfwTerminate();
     *
     * This function destroys all remaining windows and releases
     * all resources allocated by GLFW.
     *
     */
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
}
