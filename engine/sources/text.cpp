#include "text.h"

#include <filesystem>

using namespace ECS;

Types::ShaderName Text::textShader;
Types::VAO        Text::textVao;
Types::Buffer     Text::textVbo;
std::map<char, Types::Character> Text::characters;

void Text::Init()
{
    //Text::textVao.Init();
    Text::textVbo = Types::Buffer(GL_ARRAY_BUFFER);
    Text::textVao.Init();;

    Logger::Log(LogLevel::INFO, "Text initialized");

}

void Text::Load(std::string font, unsigned int fontSize)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        Logger::Log(LogLevel::ERROR, "Could not init FreeType library");
        return;
    }

    Shader::NewShader("TextShader",
                    std::filesystem::absolute("engine/shaders/text.vs"), 
                    std::filesystem::absolute("engine/shaders/text.fs"));
    textShader = "TextShader";
    Shader::Use(textShader);

    // find path to font
    std::string font_name = std::filesystem::absolute("assets/fonts/" + font);
    if (font_name.empty()) {
        Logger::Log(LogLevel::ERROR, "Could not find font");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        Logger::Log(LogLevel::ERROR, "Could not load font");
        return;
    }
    else {
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
                Logger::Log(LogLevel::ERROR, "Could not load glyph");
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            ECS::Texture::BindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            characters.insert(std::pair<char, Character>(c, character));
        }
        ECS::Texture::BindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // configure VAO/VBO for texture quads
    textVao.Bind();
    textVbo.Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    textVbo.Unbind();
    textVao.Unbind();
}

void Text::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    Shader::Use(textShader);
    unsigned int textShaderId = Shader::GetId(textShader);

    glUniform3f(glGetUniformLocation(textShaderId, "textColor"), color.x, color.y, color.z);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Screen::GetWidth()), 0.0f, static_cast<float>(Screen::GetHeight()));
    glUniformMatrix4fv(glGetUniformLocation(textShaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    textVao.Bind();

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // update VBO for each character
        
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVbo.id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

/*
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
