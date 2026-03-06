// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/cubemap.hpp>

using namespace brenta;

const Texture::Config Cubemap::default_config = {
  .type   = Texture::Type::None,
  .target = Texture::Target::TextureCubeMap,
  .properties = {
    .wrapping      = Texture::Wrapping::ClampToEdge,
    .filtering_min = Texture::Filtering::Linear,
    .filtering_mag = Texture::Filtering::Linear,
    .has_mipmap    = Gl::False,
    .flipped       = false,
  },
};
