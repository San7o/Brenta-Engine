#ifndef GL_H
#define GL_H

class GL
{
public:
    static void LoadOpenGL(GLADloadproc loadproc, int SCR_WIDTH,
                           int SCR_HEIGHT);
    static void SetPoligonMode(GLboolean enable);
    static void SetViewport(int x, int y, int SCR_WIDTH, int SCR_HEIGHT);
    static void SetColor(float r, float g, float b, float a);
    static void DrawArrays(GLenum mode, int first, int count);
    static void Clear();
};
#endif
