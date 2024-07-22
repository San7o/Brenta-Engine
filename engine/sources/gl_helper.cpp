#include <iostream>
#include "gl_helper.h"
#include "engine_logger.h"
#include "screen.h"
#include "text.h"

using namespace ECS;

void GL::LoadOpenGL()
{

    GLADloadproc loadproc = (GLADloadproc)Screen::GetProcAddress();
    if (!gladLoadGLLoader(loadproc))
    {
        Logger::Log(Types::LogLevel::ERROR, "Failed to initialize GLAD");
        exit(-1);
    }

    int SCR_WIDTH = Screen::GetWidth();
    int SCR_HEIGHT = Screen::GetHeight();

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); /* Set viewport */
    glEnable(GL_DEPTH_TEST);                 /* Enable depth testing */
    Logger::Log(Types::LogLevel::INFO, "Enabled GL_DEPTH_TEST");


    /* Enable blending for transparency */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    Logger::Log(Types::LogLevel::INFO, "Enabled GL_BLEND (transparency)");


    /* Enable face culling, draw only visible triangles
     * based on their orientation (defined clockwise or counterclockwise) */
    glEnable(GL_CULL_FACE);
    Logger::Log(Types::LogLevel::INFO,
                    "Enabled GL_CULL_FACE (draw only visible triangles)");


    /* Enable multisampling
     * Only works for a multisample buffer. */
    glEnable(GL_MULTISAMPLE);
    Logger::Log(Types::LogLevel::INFO, "Enabled GL_MULTISAMPLE");

    /* Init text class */
    Text::Init();
    Text::Load("arial.ttf", 48);

    GLenum errcode = GL::glCheckError();
    if (!errcode) Logger::Log(Types::LogLevel::INFO, "OpenGl loaded");
}

void GL::SetPoligonMode(GLboolean enable)
{
    if (enable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        Logger::Log(Types::LogLevel::INFO,
                        "Enabled GL_POLYGON_MODE (wireframe)");
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        Logger::Log(Types::LogLevel::INFO,
                        "Disabled GL_POLYGON_MODE (fill)");
    }
}

void GL::SetViewport(int x, int y, int SCR_WIDTH, int SCR_HEIGHT)
{
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); /* Set viewport */
    Logger::Log(Types::LogLevel::INFO, "Set viewport: y = " +
                std::to_string(SCR_WIDTH) + "x = " + std::to_string(SCR_HEIGHT));
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

GLenum GL::glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:      error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:     error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:    error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:   error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:     error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                        error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }

        error += " | " + std::string(file) +
                " (" + std::to_string(line) + ")";
        Logger::Log(Types::LogLevel::ERROR, error);
    }
    return errorCode;
}
