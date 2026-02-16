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
  Color(float r, float g, float b, float a = 1.0f)
    : r(r), g(g), b(b), a(a) {}
  
  static const inline Color grey()
  { return Color(0.2f, 0.2f, 0.207f, 1.0f); }
  static const inline Color yellow()
  { return Color(1.0f, 1.0f, 0.0f, 1.0f); }

};

  
} // namespace brenta
