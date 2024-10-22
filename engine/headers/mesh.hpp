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

#pragma once

#include "buffer.hpp"
#include "gl_helper.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vao.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

namespace brenta
{

namespace types
{

/**
 * @brief The Vertex struct represents a vertex of a 3D model
 *
 * A vertex is a point in 3D space that has a position, a normal
 * and texture coordinates.
 */
struct vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

/**
 * @brief The Texture struct represents a texture of a 3D model
 *
 * A texture is an image that is applied to a 3D model to give
 * it a more realistic look. A texture can be of different types
 * like diffuse, specular, normal, etc.
 */
struct texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

} // namespace types

/**
 * @brief The Mesh class represents a 3D model
 *
 * A mesh is a collection of vertices, indices and textures
 * that represent a 3D model. The mesh can be drawn using
 * a shader and calling the Draw method.
 */
class mesh
{
  public:
    /**
     * @brief vertices of the mesh
     */
    std::vector<types::vertex> vertices;
    /**
     * @brief indices of the mesh
     *
     * Indicies are used to reduce the memory footprint
     * required to represent a 3D model. They are indices
     * in the vertex array and there can be multiple indices
     * for a vertex.
     */
    std::vector<unsigned int> indices;
    /**
     * @brief textures of the mesh
     */
    std::vector<types::texture> textures;
    /**
     * @brief Type of texture wrapping
     *
     * Options are:
     * - GL_REPEAT: The default behavior for textures.
     *              Repeats the texture image.
     * - GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors
     *              the image with each repeat.
     * - GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1.
     *              The result is that higher coordinates become
     *              clamped to the edge, resulting in a stretched
     *              edge pattern.
     * - GL_CLAMP_TO_BORDER: Coordinates outside the range are
     *              now given a user-specified border color.
     */
    GLint wrapping;
    /**
     * @brief Minifying texture filtering
     *
     * Type of texture filtering used when minifying (scaling
     * down) a texture. Options are:
     * - GL_NEAREST: select the closest color to the texture coordinate
     * - GL_LINEAR: interpolates the neighbouring pixels to get an
     *              approximate color.
     */
    GLint filtering_min;
    /**
     * @brief Magnifying texture filtering
     *
     * Type of texture filtering used when magnifying (scaling
     * up) a texture. Options are:
     * - GL_NEAREST: select the closest color to the texture coordinate
     * - GL_LINEAR: interpolates the neighbouring pixels to get an
     *              approximate color.
     */
    GLint filtering_mag;
    /**
     * @brief Should the texture have a mipmap?
     */
    GLboolean has_mipmap;
    /**
     * @brief Type of mipmap minifying texture filtering
     *
     * Type of texture filtering used when minifying (scaling
     * down) a texture with mipmaps. Options are:
     * - GL_NEAREST: select the closest color to the texture coordinate
     * - GL_LINEAR: interpolates the neighbouring pixels to get an
     *              approximate color.
     * - GL_NEAREST_MIPMAP_NEAREST: selects the mipmap that most
     *              closely matches the size of the pixel being textured
     *              and uses the GL_NEAREST criterion (the texture element
     *              nearest to the center of the pixel) to produce a texture
     *              value.
     * - GL_LINEAR_MIPMAP_NEAREST: selects the mipmap that most closely
     *              matches the size of the pixel being textured and uses
     *              the GL_LINEAR criterion (a weighted average of the four
     *              texture elements that are closest to the center of the
     *              pixel) to produce a texture value.
     * - GL_NEAREST_MIPMAP_LINEAR: selects the two mipmaps that most closely
     *              match the size of the pixel being textured and uses the
     *              GL_NEAREST criterion (the texture element nearest to the
     *              center of the pixel) to produce a texture value from each
     *              mipmap. The final texture value is a weighted average of
     *              those two values.
     * - GL_LINEAR_MIPMAP_LINEAR: selects the two mipmaps that most closely
     *              match the size of the pixel being textured and uses the
     *              GL_LINEAR criterion (a weighted average of the texture
     *              elements that are closest to the center of the pixel) to
     *              produce a texture value from each mipmap. The final texture
     *              value is a weighted average of those two values.
     */
    GLint mipmap_min;
    /**
     * @brief Type of mipmap magnifying texture filtering
     *
     * Type of texture filtering used when magnifying (scaling
     * up) a texture with mipmaps. Options are:
     * - GL_NEAREST: select the closest color to the texture coordinate
     * - GL_LINEAR: interpolates the neighbouring pixels to get an
     *              approximate color.
     * - GL_NEAREST_MIPMAP_NEAREST: selects the mipmap that most
     *              closely matches the size of the pixel being textured
     *              and uses the GL_NEAREST criterion (the texture element
     *              nearest to the center of the pixel) to produce a texture
     *              value.
     * - GL_LINEAR_MIPMAP_NEAREST: selects the mipmap that most closely
     *              matches the size of the pixel being textured and uses
     *              the GL_LINEAR criterion (a weighted average of the four
     *              texture elements that are closest to the center of the
     *              pixel) to produce a texture value.
     * - GL_NEAREST_MIPMAP_LINEAR: selects the two mipmaps that most closely
     *              match the size of the pixel being textured and uses the
     *              GL_NEAREST criterion (the texture element nearest to the
     *              center of the pixel) to produce a texture value from each
     *              mipmap. The final texture value is a weighted average of
     *              those two values.
     * - GL_LINEAR_MIPMAP_LINEAR: selects the two mipmaps that most closely
     *              match the size of the pixel being textured and uses the
     *              GL_LINEAR criterion (a weighted average of the texture
     *              elements that are closest to the center of the pixel) to
     *              produce a texture value from each mipmap. The final texture
     *              value is a weighted average of those two values.
     */
    GLint mipmap_mag;

    /**
     * @brief Construct a new Mesh object
     *
     * @param vertices Vertices of the mesh
     * @param indices Indices of the mesh
     * @param textures Textures of the mesh
     * @param wrapping Type of texture wrapping
     * @param filtering_min Minifying texture filtering
     * @param filtering_mag Magnifying texture filtering
     * @param hasMipmap Should the texture have a mipmap?
     * @param mipmap_min Type of mipmap minifying texture filtering
     * @param mipmap_mag Type of mipmap magnifying texture filtering
     */
    mesh(std::vector<types::vertex> vertices, std::vector<unsigned int> indices,
         std::vector<types::texture> textures, GLint wrapping = GL_REPEAT,
         GLint filtering_min = GL_NEAREST, GLint filtering_mag = GL_LINEAR,
         GLboolean hasMipmap = GL_TRUE,
         GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
         GLint mipmap_max = GL_LINEAR);
    /**
     * @brief The Builder class is used to build a Mesh object
     */
    class builder;

    /**
     * @brief Draw the mesh
     *
     * @param shader_name Shader to use to draw the mesh
     */
    void draw(types::shader_name_t shader_name);

  private:
    // render data
    types::vao vao;
    types::buffer vbo;
    types::buffer ebo;
    void setup_mesh();
};

/**
 * @brief The Builder class is used to build a Mesh object
 */
class mesh::builder
{
  private:
    std::vector<types::vertex> vertices = {};
    std::vector<unsigned int> indices = {};
    std::vector<types::texture> textures = {};
    GLint wrapping = GL_REPEAT;
    GLint filtering_min = GL_NEAREST;
    GLint filtering_mag = GL_LINEAR;
    GLboolean has_mipmap = GL_TRUE;
    GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR;
    GLint mipmap_mag = GL_LINEAR;

  public:
    builder &set_vertices(std::vector<types::vertex> vertices);
    builder &set_indices(std::vector<unsigned int> indices);
    builder &set_textures(std::vector<types::texture> textures);
    builder &set_wrapping(GLint wrapping);
    builder &set_filtering_min(GLint filtering_min);
    builder &set_filtering_mag(GLint filtering_mag);
    builder &set_has_mipmap(GLboolean has_mipmap);
    builder &set_mipmap_min(GLint mipmap_min);
    builder &set_mipmap_mag(GLint mipmap_mag);

    mesh build();
};

} // namespace brenta
