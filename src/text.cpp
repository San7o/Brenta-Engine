// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/text.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace brenta;

void Text::render(const std::string &text,
                  float x, float y, float scale,
                  Color color,
                  std::shared_ptr<Font> font)
{
  
  font->shader->use();
  font->shader->set_float3("textColor",
                           255.99f * color.r,
                           255.99f * color.g,
                           255.99f * color.b);

  glm::mat4 projection =
    glm::ortho(0.0f, static_cast<float>(Window::get_width()), 0.0f,
               static_cast<float>(Window::get_height()));

  font->shader->set_mat4("projection", projection);

  glActiveTexture(GL_TEXTURE0);
  font->vao.bind();

  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Font::Character ch = font->characters[*c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    // update VBO for each character

    float vertices[6][4] = {
      {xpos, ypos + h, 0.0, 0.0},    {xpos, ypos, 0.0, 1.0},
      {xpos + w, ypos, 1.0, 1.0},

      {xpos, ypos + h, 0.0, 0.0},    {xpos + w, ypos, 1.0, 1.0},
      {xpos + w, ypos + h, 1.0, 0.0}};

    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.texture_id);

    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, font->vbo.get_id());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number
    // of 1/64 pixels)
    x += (ch.advance >> 6)
         * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  return;
}
