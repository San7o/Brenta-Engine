// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/text.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace brenta;

void Text::render(int width, int height) const
{
  this->font->shader->use();
  this->font->shader->set_float3("textColor",
                                 255.99f * this->color.r,
                                 255.99f * this->color.g,
                                 255.99f * this->color.b);

  glm::mat4 projection =
    glm::ortho(0.0f, static_cast<float>(width), 0.0f,
               static_cast<float>(height));

  this->font->shader->set_mat4("projection", projection);

  glActiveTexture(GL_TEXTURE0);
  this->font->vao.bind();

  // iterate through all characters
  int _x = this->x;
  int _y = this->y;
  std::string::const_iterator c;
  for (c = this->text.begin(); c != this->text.end(); c++)
  {
    Font::Character ch = this->font->characters[*c];

    float xpos = _x + ch.bearing.x * this->scale;
    float ypos = _y - (ch.size.y - ch.bearing.y) * this->scale;

    float w = ch.size.x * this->scale;
    float h = ch.size.y * this->scale;

    // update VBO for each character

    float vertices[6][4] = {
      {xpos, ypos + h, 0.0, 0.0},    {xpos, ypos, 0.0, 1.0},
      {xpos + w, ypos, 1.0, 1.0},

      {xpos, ypos + h, 0.0, 0.0},    {xpos + w, ypos, 1.0, 1.0},
      {xpos + w, ypos + h, 1.0, 0.0}};

    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.texture_id);

    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, this->font->vbo.get_id());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number
    // of 1/64 pixels)
    _x += (ch.advance >> 6)
      * this->scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  return;
}
