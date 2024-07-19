#ifndef GL_H
#define GL_H

#include <glad/glad.h>       /* OpenGL driver */

class GL
{
public:
    static void LoadOpenGL(GLADloadproc loadproc, int SCR_WIDTH,
                           int SCR_HEIGHT);
    static void SetPoligonMode(GLboolean enable);
    static void SetViewport(int x, int y, int SCR_WIDTH, int SCR_HEIGHT);
    static void SetColor(float r, float g, float b, float a);
    static void DrawArrays(GLenum mode, int first, int count);
    static void DrawElements(GLenum mode, int count, GLenum type, const void* indices);
    static void Clear();
    static void BindVertexArray(unsigned int n);
};
#endif