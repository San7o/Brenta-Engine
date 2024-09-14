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
 * The above copyright notice and this permission notice shall be included in
 all
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

#include "translation.hpp"

using namespace brenta::types;

translation::translation()
{
    this->view = glm::mat4(1.0f);
    this->projection = glm::mat4(1.0f);
    this->model = glm::mat4(1.0f);
}

translation::translation(glm::mat4 view, glm::mat4 projection, glm::mat4 model)
{
    this->view = view;
    this->projection = projection;
    this->model = model;
}

void translation::set_view(glm::mat4 view)
{
    this->view = view;
}

void translation::set_projection(float fov, float near, float far)
{
    this->projection = glm::perspective(
        glm::radians(fov),
        (float) screen::get_width() / (float) screen::get_height(), near, far);
}

void translation::set_projection(glm::mat4 projection)
{
    this->projection = projection;
}

void translation::set_model(glm::mat4 new_model)
{
    this->model = new_model;
}

void translation::translate(glm::vec3 translation)
{
    this->model = glm::translate(this->model, translation);
}

/* Note: the order of rotations is important */
void translation::rotate(glm::vec3 rotation)
{
    this->model = glm::rotate(this->model, glm::radians(rotation.x),
                              glm::vec3(1.0f, 0.0f, 0.0f));
    this->model = glm::rotate(this->model, glm::radians(rotation.y),
                              glm::vec3(0.0f, 1.0f, 0.0f));
    this->model = glm::rotate(this->model, glm::radians(rotation.z),
                              glm::vec3(0.0f, 0.0f, 1.0f));
}

void translation::scale(float scale)
{
    this->model = glm::scale(this->model, glm::vec3(scale));
}

void translation::set_shader(types::shader_name_t shader_name)
{
    shader::use(shader_name);

    shader::set_mat4(shader_name, "view", this->view);
    shader::set_mat4(shader_name, "projection", this->projection);
    shader::set_mat4(shader_name, "model", this->model);
}
