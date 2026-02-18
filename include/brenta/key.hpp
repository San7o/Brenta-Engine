// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <bitset>

namespace brenta
{

enum class Key : int
{
  None,
  Unknown,

  // Printable keys
  Space,
  Apostrophe,
  Comma,
  Minus,
  Period,
  Slash,
  D0,
  D1,
  D2,
  D3,
  D4,
  D5,
  D6,
  D7,
  D8,
  D9,
  Semicolon,
  Equal,
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  LeftBracket,
  Backslash,
  RightBracket,
  GraveAccent,
  World1,
  World2,

  // Function keys
  Escape,
  Enter,
  Tab,
  Backspace,
  Insert,
  Delete,
  Right,
  Left,
  Down,
  Up,
  PageUp,
  PageDown,
  Home,
  End,
  CapsLock,
  ScrollLock,
  NumLock,
  PrintScreen,
  Pause,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  F16,
  F17,
  F18,
  F19,
  F20,
  F21,
  F22,
  F23,
  F24,
  F25,

  // Keypad
  KP0,
  KP1,
  KP2,
  KP3,
  KP4,
  KP5,
  KP6,
  KP7,
  KP8,
  KP9,
  KPDecimal,
  KPDivide,
  KPMultiply,
  KPSubtract,
  KPAdd,
  KPEnter,
  KPEqual,

  // Modifiers
  LeftShift,
  LeftControl,
  LeftAlt,
  LeftSuper,
  RightShift,
  RightControl,
  RightAlt,
  RightSuper,
  Menu,
  
  Last = Menu
};

enum class KeyAction
{
  Release,
  Press,
  Repeat,
  Unknown,
};

class KeyMods
{
public:
  
  enum class Mod : std::size_t
  {
    Shift,
    Control,
    Alt,
    Super,
    CapsLock,
    NumLock,
  };

  KeyMods() = default;
  
  inline bool shift() const
  { return this->mods.test((std::size_t) Mod::Shift); }
  inline bool control() const
  { return this->mods.test((std::size_t) Mod::Control); }
  inline bool alt() const
  { return this->mods.test((std::size_t) Mod::Alt); }
  inline bool super() const
  { return this->mods.test((std::size_t) Mod::Super); }
  inline bool caps_lock() const
  { return this->mods.test((std::size_t) Mod::CapsLock); }
  inline bool num_lock() const
  { return this->mods.test((std::size_t) Mod::NumLock); }

  KeyMods &set_shift()
  { this->mods.set((std::size_t) Mod::Shift);    return *this; }
  KeyMods &set_control()
  { this->mods.set((std::size_t) Mod::Control);  return *this; }
  KeyMods &set_alt()
  { this->mods.set((std::size_t) Mod::Alt);      return *this; }
  KeyMods &set_super()
  { this->mods.set((std::size_t) Mod::Super);    return *this; }
  KeyMods &set_caps_lock()
  { this->mods.set((std::size_t) Mod::CapsLock); return *this; }
  KeyMods &set_num_lock()
  { this->mods.set((std::size_t) Mod::NumLock);  return *this; }
  
private:
  
  std::bitset<8> mods;
};
  
} // namespace brenta
