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

#include "text.hpp"

#include "engine_logger.hpp"

#include <filesystem>

using namespace brenta;
using namespace brenta::types;

types::shader_name_t text::text_shader;
types::vao text::text_vao;
types::buffer text::text_vbo;
std::map<char, types::character> text::characters;

void text::init()
{
    text::text_vbo = types::buffer(GL_ARRAY_BUFFER);
    text::text_vao.init();

    INFO("Text initialized");
}

void text::load(std::string font, unsigned int font_size)
{
    if (text_vao.get_vao() == 0)
    {
        ERROR("Text not initialized");
        return;
    }
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        ERROR("Could not init FreeType library");
        return;
    }

    shader::create("TextShader", GL_VERTEX_SHADER,
                std::filesystem::absolute("engine/shaders/text.vs"),
                GL_FRAGMENT_SHADER,
                std::filesystem::absolute("engine/shaders/text.fs"));
    text_shader = "TextShader";
    shader::use(text_shader);

    // find path to font
    std::string font_name = std::filesystem::absolute("assets/fonts/" + font);
    if (font_name.empty())
    {
        ERROR("Could not find font");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face))
    {
        ERROR("Could not load font");
        return;
    }
    else
    {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                ERROR("Could not load glyph");
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            texture::bind_texture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                         face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            character character_ = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)};
            characters.insert(std::pair<char, character>(c, character_));
        }
        texture::bind_texture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // configure VAO/VBO for texture quads
    text_vao.bind();
    text_vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    text_vbo.unbind();
    text_vao.unbind();
}

void text::render_text(std::string text, float x, float y, float scale,
                      glm::vec3 color)
{
    if (text_vao.get_vao() == 0)
    {
        ERROR("Text not initialized");
        return;
    }

    shader::use(text_shader);
    unsigned int textShaderId = shader::get_id(text_shader);

    glUniform3f(glGetUniformLocation(textShaderId, "textColor"), color.x,
                color.y, color.z);

    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(screen::get_width()), 0.0f,
                   static_cast<float>(screen::get_height()));
    glUniformMatrix4fv(glGetUniformLocation(textShaderId, "projection"), 1,
                       GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    text_vao.bind();

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        character ch = characters[*c];

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
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo.id);
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

    /*
    // Render a test triangle
    Shader::Use(textShader);

    float vertices[6][4] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 1.0f,

            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 0.0f
    };
    textVao.Bind();
    textVbo.Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    textVbo.Unbind();

    glDrawArrays(GL_TRIANGLES, 0, 6);
    */
}
