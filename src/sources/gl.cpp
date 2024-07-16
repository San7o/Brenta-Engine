#include <glad/glad.h>       /* OpenGL driver */
#include <iostream>
#include "gl.h"

void GL::LoadOpenGL(GLADloadproc loadproc, int SCR_WIDTH, int SCR_HEIGHT)
{
    if (!gladLoadGLLoader(loadproc))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); /* Set viewport */
    glEnable(GL_DEPTH_TEST);                 /* Enable depth testing */

    /* Enable blending for transparency */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    /* Enable face culling, draw only visible triangles
     * based on their orientation (defined clockwise or counterclockwise) */
    glEnable(GL_CULL_FACE);

}

void GL::SetPoligonMode(GLboolean enable)
{
    if (enable)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GL::SetViewport(int x, int y, int SCR_WIDTH, int SCR_HEIGHT)
{
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); /* Set viewport */
}

void GL::SetColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a); /* Set clear color */
}

void GL::DrawArrays(GLenum mode, int first, int count)
{
    glDrawArrays(mode, first, count);
}

void GL::DrawElements(GLenum mode, int count, GLenum type, const void* indices)
{
    glDrawElements(mode, count, type, indices);
}

void GL::Clear()
{
    /* Clear color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL::BindVertexArray(unsigned int n)
{
    glBindVertexArray(n);
}
