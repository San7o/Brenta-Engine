/*
 * Helper functions to interact with OpenGL.
 */

#pragma once

#ifndef GL_HELPER_H
#define GL_HELPER_H

#ifndef __glad_h_
#include <glad/glad.h>       /* OpenGL driver */
#endif

namespace ECS {

class GL {
public:
    static void LoadOpenGL();
    static void SetPoligonMode(GLboolean enable);
    static void SetViewport(int x, int y, int SCR_WIDTH, int SCR_HEIGHT);
    static void SetColor(float r, float g, float b, float a);
    static void DrawArrays(GLenum mode, int first, int count);
    static void DrawElements(GLenum mode, int count, GLenum type, const void* indices);
    static void Clear();
    static void BindVertexArray(unsigned int n);
    static GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)
};

} // namespace ECS

#endif
