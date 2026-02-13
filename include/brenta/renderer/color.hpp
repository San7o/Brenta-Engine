// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

namespace brenta
{

class Color
{
public:

  float r, g, b, a;
  
  Color() = default;
  Color(float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a) {}
  
  static const inline Color grey()
  { return Color(0.2f, 0.2f, 0.207f, 1.0f); }

};

  
} // namespace brenta
