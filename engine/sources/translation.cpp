/*
 * MIT License
 * 
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */ 

#include "translation.h"

using namespace ECS::Types;

Translation::Translation()
{
    this->view = glm::mat4(1.0f);
    this->projection = glm::mat4(1.0f);
    this->model = glm::mat4(1.0f);
}

void Translation::setView(glm::mat4 view)
{
    this->view = view;
}

void Translation::setProjection(float fov, float near, float far)
{
    this->projection = glm::perspective(glm::radians(fov), 
        (float) ECS::Screen::GetWidth() / (float) ECS::Screen::GetHeight(),
        near, far);
}

void Translation::setProjection(glm::mat4 projection)
{
    this->projection = projection;
}

void Translation::setModel(glm::mat4 new_model)
{
    this->model = new_model;
}

void Translation::translate(glm::vec3 translation)
{
    this->model = glm::translate(this->model, translation);
}

void Translation::rotate(glm::vec3 rotation)
{
    this->model = glm::rotate(this->model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    this->model = glm::rotate(this->model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model = glm::rotate(this->model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Translation::scale(float scale)
{
    this->model = glm::scale(this->model, glm::vec3(scale));
}

void Translation::setShader(Types::ShaderName shader_name)
{
    Shader::Use(shader_name);

    Shader::SetMat4(shader_name, "view", this->view);
    Shader::SetMat4(shader_name, "projection", this->projection);
    Shader::SetMat4(shader_name, "model", this->model);

}
