/*
 *
 *     Created on: 2024-07-03
 *     Author: Giovanni Santini 
 *     License: MIT
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glm/glm.hpp>

#ifndef GLAD_H
#include <glad/glad.h>
#endif


class Shader
{

public:
    unsigned int ID; /* Program ID */

    /* Constructor reads and builds the shader */
    Shader(const char* vertexPath, const char* fragmentPath);

    /* Use/activate the shader */
    void use();

    /* Utility uniform functions */
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const GLchar* name, glm::mat4 value) const;
    void setVec3(const GLchar* name, float x, float y, float z) const;
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
